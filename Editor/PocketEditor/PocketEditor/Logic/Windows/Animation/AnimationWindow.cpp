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
    guiRoot.CreateSystem<VirtualTreeListSystem>();
    guiRoot.CreateSystem<VirtualTreeListSpawnerSystem>();
    guiRoot.CreateSystem<SelectedColorerSystem>();
    guiRoot.CreateSystem<DraggableSystem>();
    
    animationRoot = nullptr;
    
    Zoom = 1.0f;
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
        old->CreateSystem<SelectableCollection<EditorObject>>()->SelectionChanged.Unbind(this, &AnimationWindow::SelectionChanged);
    }
    if (current) {
        selectables = current->CreateSystem<SelectableCollection<EditorObject>>();
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

    if (animationRoot) {
        animationRoot->Remove();
        animationRoot = nullptr;
    }
    
    animation = nullptr;
    animationObject = nullptr;
}

void AnimationWindow::SelectionChanged(SelectableCollection<EditorObject>* selectables) {
    for(auto o : selectables->Selected()) {
        GameObject* gameObject = o->GetComponent<EditorObject>()->gameObject;
        Animation* a = gameObject->GetComponent<Animation>();
        if (a) {
            Clear();
            
            animation = a;
            animationObject = o;
            
            animator = gameObject->GetComponent<Animator>();
            
            if (animator) {
                animator->Time.Changed.Bind(this, &AnimationWindow::TimeChanged);
            }
            
            animationRoot = context->World().CreateRoot();
            timelines.clear();
            
            for(auto& t : animation->GetTargets()) {
                GameObject* target = animationRoot->CreateChild();
                auto& ti = timelines[target];
                ti.name = "GameObject";
                
                for(auto c : t.components) {
                    GameObject* component = target->CreateChild();
                    auto& cc = timelines[component];
                    cc.name = c.first;
                    
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
    
    timeline = gui.CreateControl(window, "Box", {0,window->GetComponent<Sizeable>()->Size().y-75}, {window->GetComponent<Sizeable>()->Size().x, 25});
    for (int i=0; i<20; i++) {
        Vector2 size = {2,25};
        float xPos = TimeToPosition(i);
        GameObject* indicator = gui.CreateControl(timeline, "TextBox", {xPos-size.x* 0.5f, 0}, size);
        indicator->GetComponent<Colorable>()->Color = Colour::Black();
        indicator->RemoveComponent<Touchable>();
    }
    timeline->GetComponent<Touchable>()->Down.Bind([ this] (TouchData e) {
        if (!animator) return;
        this->context->UpdateLoop.Bind(this, &AnimationWindow::UpdateTime, e.Index);
    });
    
    timeline->GetComponent<Touchable>()->Up.Bind([ this] (TouchData e) {
        if (!animator) return;
        this->context->UpdateLoop.Unbind(this, &AnimationWindow::UpdateTime, e.Index);
    });
    

    GameObject* pivot;
    GameObject* listBox = gui.CreateListbox(window, "Box", {0,0}, {window->GetComponent<Sizeable>()->Size().x,window->GetComponent<Sizeable>()->Size().y-75}, &pivot);
    listBox->RemoveComponent<Sprite>();

    treeView = pivot->AddComponent<VirtualTreeList>();
    treeView->ShowRoot = false;
    treeView->Root = nullptr;
    treeView->Pivot = listBox;
    treeView->PredicateFunction = [] (GameObject* object) {
        if (object->Parent() && object->Parent()->GetComponent<Cloner>()) return false;
        return true;
    };
    treeView->ItemIndent = 0;
    
    auto spawner = pivot->AddComponent<VirtualTreeListSpawner>();
    
    spawner->HasChildren = [] (GameObject* object) {
        if (object->GetComponent<Cloner>()) return false;
        return !object->Children().empty();
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
                for(auto time : timeline->GetTimeNodes()) {
                    Vector2 size = {10,n.height};
                    float xPos = TimeToPosition(time);
                    gui.CreateControl(box, "TextBox", {xPos-size.x* 0.5f, 0}, size);
                }
            }
            
        }
    };
    
    spawner->OnRemove = [this] (auto& n) {
    };
    
    window->GetComponent<Transform>()->Position += {300,200};
    
    
    timeIndicator = gui.CreateControl(window, "Box", {0,0}, {2, window->GetComponent<Sizeable>()->Size().y-50});
    timeIndicator->GetComponent<Colorable>()->Color = Colour::Black();
    
    
}
