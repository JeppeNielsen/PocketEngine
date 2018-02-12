//
//  AnimationWindow.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 17/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "AnimationWindow.hpp"
#include "VirtualTreeListSystem.hpp"
#include "VirtualTreeListSpawnerSystem.hpp"
#include "SelectedColorer.hpp"
#include "Selectable.hpp"
#include "DroppableSystem.hpp"
#include "EditorDropTarget.hpp"
#include "Cloner.hpp"
#include "Animation.hpp"

std::string AnimationWindow::Name() { return "Animation"; }

Vector2 AnimationWindow::Size() {
    return { 600,300 };
}

void AnimationWindow::OnInitialize() {
    GameObject& guiRoot = context->GuiRoot();
//    guiRoot.CreateSystem<VirtualTreeListSystem>();
//    guiRoot.CreateSystem<VirtualTreeListSpawnerSystem>();
//    guiRoot.CreateSystem<SelectedColorerSystem>();
//    guiRoot.CreateSystem<DraggableSystem>();
    
    animationRoot = nullptr;
    
    Zoom = 1.0f;
    IsRecording = false;
    IsRecording.Changed.Bind([this] () {
        if (IsRecording()) {
            context->UpdateLoop.Bind(this, &AnimationWindow::Record);
            recordButton->GetComponent<Colorable>()->Color = Colour::Red();
        } else {
            context->UpdateLoop.Unbind(this, &AnimationWindow::Record);
            recordButton->GetComponent<Colorable>()->Color = Colour::White();
        }
    });
    
    isScrupping = false;
    
    nonRecordableComponents = {
        GameIdHelper::GetComponentID<Animation>(),
        GameIdHelper::GetComponentID<Mesh>(),
        GameIdHelper::GetComponentID<Animator>(),
    };
}

void AnimationWindow::ActiveWorldChanged(OpenWorld* old, OpenWorld* current) {
    if (old) {
        old->EditorRoot.Changed.Unbind(this, &AnimationWindow::EditorRootChanged, old);
    }
    if (current) {
        current->EditorRoot.Changed.Bind(this, &AnimationWindow::EditorRootChanged, current);
    }
    ChangeEditorRoot(old ? old->EditorRoot : 0, current ? current->EditorRoot : 0);
}

void AnimationWindow::EditorRootChanged(OpenWorld *world) {
    ChangeEditorRoot(world->EditorRoot.PreviousValue(), world->EditorRoot);
}

void AnimationWindow::ChangeEditorRoot(Pocket::GameObject *old, Pocket::GameObject *current) {
    Clear();
    if (old) {
        old->GetSystem<SelectableCollection<EditorObject>>()->SelectionChanged.Unbind(this, &AnimationWindow::SelectionChanged);
    }
    if (current) {
        selectables = current->GetSystem<SelectableCollection<EditorObject>>();
        selectables->SelectionChanged.Bind(this, &AnimationWindow::SelectionChanged);
    } else {
        selectables = 0;
    }
}

void AnimationWindow::TimeChanged() {
    timeIndicator->GetComponent<Transform>()->Position = { this->TimeToPosition(animator->Time) , 0};
}

void AnimationWindow::Clear() {
    if (animator) {
        animator->Time.Changed.Unbind(this, &AnimationWindow::TimeChanged);
        animator = nullptr;
    }

    
    
    animation = nullptr;
    animationObject = nullptr;
}

void AnimationWindow::Refresh() {
    if (animationRoot) {
        animationRoot->Remove();
        animationRoot = nullptr;
    }
    animationRoot = context->World().CreateScene();
    timelines.clear();
    
    for(auto& t : animation->GetTargets()) {
        GameObject* target = animationRoot->CreateChild();
        auto& ti = timelines[target];
        ti.name = "GameObject";
        ti.path = t.path;
        
        for(auto c : t.components) {
            GameObject* component = target->CreateChild();
            auto& cc = timelines[component];
            cc.name = c.first;
            cc.path = t.path;
            
            for(auto f : c.second) {
                GameObject* field = component->CreateChild();
                auto& ff = timelines[field];
                ff.name = f.first;
                ff.timeline = f.second.get();
            }
        }
    }
    
    treeView->Root = animationRoot;
    treeView->SetNodeExpanded(animationRoot, true);
}

void AnimationWindow::StoreValues() {
    previousValues.clear();
    for(auto o : selectables->Selected()) {
        GameObject* gameObject = o->GetComponent<EditorObject>()->gameObject;
        for(auto& type : gameObject->GetComponentTypes()) {
            for(auto& field : type.fields) {
                std::stringstream s;
                minijson::writer_configuration config;
                minijson::object_writer writer(s, config);
                field->Serialize(writer);
                previousValues.push_back(s.str());
            }
        }
    }
}

void AnimationWindow::SelectionChanged(SelectableCollection<EditorObject>* selectables) {
    StoreValues();
    for(auto o : selectables->Selected()) {
        GameObject* gameObject = o->GetComponent<EditorObject>()->gameObject;
        Animation* a = gameObject->GetComponent<Animation>();
        if (a) {
            Clear();
            
            animation = a;
            animationObject = gameObject;
            
            animator = gameObject->GetComponent<Animator>();
            
            if (animator) {
                animator->Time.Changed.Bind(this, &AnimationWindow::TimeChanged);
            }
            Refresh();
            break;
        }
    }
}

float AnimationWindow::TimeToPosition(float time) {
    return time * 100.0f * Zoom();
}

float AnimationWindow::PositionToTime(float position) {
    return position / (100.0f * Zoom());
}

void AnimationWindow::UpdateTime(float dt, int index) {
    Vector3 worldPosition = context->EngineContext().InputDevice().GetTouchPosition(index);
    
    Vector3 localPosition = timeline->GetComponent<Transform>()->WorldInverse().TransformPosition(worldPosition);
    animator->Time = this->PositionToTime(localPosition.x);
}

void AnimationWindow::OnCreate() {
    Gui& gui = context->Gui();
    
    

    GameObject* pivot;
    GameObject* listBox = gui.CreateListbox(window, "Box", {0,0}, {window->GetComponent<Sizeable>()->Size().x,window->GetComponent<Sizeable>()->Size().y-100}, &pivot);
    listBox->RemoveComponent<Sprite>();
    gui.AddLayouter(listBox, 20, {2000,2000}, {2000,2000});

    treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->ShowRoot = false;
    treeView->Root = nullptr;
    treeView->Pivot = listBox;
    treeView->PredicateFunction = [] (GameObject* object) {
        if (object->Hierarchy().Parent() && object->Hierarchy().Parent()->GetComponent<Cloner>()) return false;
        return true;
    };
    treeView->ItemIndent = 0;
    treeView->ExpandedHashFunction = [this] (GameObject* object) -> std::string {
        auto it = timelines.find(object);
        if (it == timelines.end()) return "";
        std::stringstream s;
        for(int index : it->second.path.indices) {
            s<<index;
        }
        s<<it->second.name;
        s<<it->second.timeline;
        return s.str();
    };
    
    auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
    
    spawner->HasChildren = [] (GameObject* object) {
        if (object->GetComponent<Cloner>()) return false;
        return !object->Hierarchy().Children().empty();
    };
    
    spawner->OnCreate = [&, this](VirtualTreeListSpawner::SpawnedNode& n) {
        
        GameObject* box = gui.CreateControl(n.parent, "Box", 0, {200,n.height});
        if (n.hasChildren) {
            n.foldOutButton = gui.CreateControl(box, "TextBox", 0, {25,25});
        }
        
        auto t = timelines.find(n.node);
        if (t!=timelines.end()) {
        
            auto l = gui.CreateLabel(box, {(n.hasChildren ? 25.0f : 0.0f),0}, {200-(n.hasChildren ? 0 : 25.0f),25}, 0, t->second.name, 12);
            l->GetComponent<Colorable>()->Color = Colour::Black();
            l->GetComponent<Label>()->HAlignment = Font::HAlignment::Left;
            l->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
            
            auto timeline = t->second.timeline;
            
            if (timeline) {
                int index = 0;
                for(auto time : timeline->GetTimeNodes()) {
                    Vector2 size = {10,n.height};
                    float xPos = TimeToPosition(time);
                    GameObject* node = gui.CreateControl(box, "TextBox", {xPos-size.x* 0.5f, 0}, size);
                    node->AddComponent<Draggable>()->Movement = Draggable::MovementMode::XAxis;
                    node->GetComponent<Touchable>()->Up.Bind([index, node, size, timeline, this] (TouchData d) {
                        timeline->ChangeNodeTime(index, this->PositionToTime(node->GetComponent<Transform>()->Position().x + size.x * 0.5f));
                    });
                    index++;
                }
            }
            
        }
    };
    
    spawner->OnRemove = [this] (auto& n) {
    };
    
    window->GetComponent<Transform>()->Position += {300,200};
    
    
    recordButton = gui.CreateControl(window, "Box", {0,window->GetComponent<Sizeable>()->Size().y-75}, {25, 25});
    recordButton->GetComponent<Touchable>()->Click.Bind([this] (auto e){
        IsRecording = !IsRecording();
    });
    gui.AddLayouter(recordButton, 20, 20, 20);
    
    timeline = gui.CreateControl(window, "Box", {0,window->GetComponent<Sizeable>()->Size().y-100}, {window->GetComponent<Sizeable>()->Size().x, 25});
    for (int i=0; i<20; i++) {
        Vector2 size = {2,25};
        float xPos = TimeToPosition(i);
        GameObject* indicator = gui.CreateControl(timeline, "TextBox", {xPos-size.x* 0.5f, 0}, size);
        indicator->GetComponent<Colorable>()->Color = Colour::Black();
        indicator->RemoveComponent<Touchable>();
    }
    gui.AddLayouter(timeline, 25, {2000,25}, {2000,25});
    timeline->GetComponent<Touchable>()->Down.Bind([ this] (TouchData e) {
        if (!animator) return;
        this->context->UpdateLoop.Bind(this, &AnimationWindow::UpdateTime, e.Index);
        isScrupping = true;
    });
    
    timeline->GetComponent<Touchable>()->Up.Bind([ this] (TouchData e) {
        if (!animator) return;
        this->context->UpdateLoop.Unbind(this, &AnimationWindow::UpdateTime, e.Index);
        this->StoreValues();
        isScrupping = false;
    });

    
    
    timeIndicator = gui.CreateControl(timeline, "Box", {0,0}, {2, window->GetComponent<Sizeable>()->Size().y-50});
    timeIndicator->GetComponent<Colorable>()->Color = Colour::Black();
    timeIndicator->RemoveComponent<Touchable>();
    
}

void AnimationWindow::Record(float dt) {
    if (!animation) return;
    if (!animator) return;
    if (isScrupping) return;
    std::vector<std::string> currentValues;
    int index = 0;
    bool anyChanges = false;
    for(auto o : selectables->Selected()) {
        GameObject* gameObject = o->GetComponent<EditorObject>()->gameObject;
        for(auto& type : gameObject->GetComponentTypes()) {
            for(auto& field : type.fields) {
                std::stringstream s;
                minijson::writer_configuration config;
                minijson::object_writer writer(s, config);
                field->Serialize(writer);
                currentValues.push_back(s.str());
                
                if (previousValues[index]!=currentValues[index]) {
                    int componentId;
                    if (gameObject->World()->Storage().TryGetComponentIndex(type.name, componentId)) {
                        if (std::find(nonRecordableComponents.begin(), nonRecordableComponents.end(), componentId) == nonRecordableComponents.end()) {
                            animation->AddNode(animationObject, gameObject, componentId, field->name, animator->Time);
                            anyChanges = true;
                        }
                    }
                }
                index++;
            }
        }
    }
    
    previousValues = currentValues;

    if (anyChanges) {
        Refresh();
    }
}
