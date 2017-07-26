//
//  testWindowSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 24/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"
#include "Gui.hpp"
#include "DroppableSystem.hpp"

using namespace Pocket;

class Panel;

struct PanelArea {

    PanelArea() {
        Direction = DirectionMode::Fill;
    }

    enum class DirectionMode { Fill, Top, Bottom, Left, Right };
    Property<DirectionMode> Direction;
    
    std::vector<GameObject*> Panels;
};

struct Panel {
    Property<GameObject*> CurrentArea;

    Panel() {
        CurrentArea = nullptr;
    }
    
    void operator = (const Panel& panel) {
        CurrentArea = nullptr;
    }
};

struct PanelDropper {
    GameObject* panel;
};

struct PanelSystem : public GameSystem<Panel> {
    
    void ObjectAdded(GameObject* object) override {
        object->GetComponent<Panel>()->CurrentArea.Changed.Bind(this, &PanelSystem::CurrentAreaChanged, object);
        CurrentAreaChanged(object);
    }

    void ObjectRemoved(GameObject* object) override {
        object->GetComponent<Panel>()->CurrentArea.Changed.Unbind(this, &PanelSystem::CurrentAreaChanged, object);
    }
    
    void Update(float dt) override {
        for(auto o : Objects()) {
            Panel* panel = o->GetComponent<Panel>();
            if (panel->CurrentArea()) {
                o->GetComponent<Transform>()->Position = panel->CurrentArea()->GetComponent<Transform>()->World().TransformPosition(0);
                o->GetComponent<Sizeable>()->Size = panel->CurrentArea()->GetComponent<Sizeable>()->Size;
            }
        }
    }
    
    void CurrentAreaChanged(GameObject* object) {
//        Panel* panel = object->GetComponent<Panel>();
//        if (object->GetComponent<Droppable>()) return;
//        if (panel->CurrentArea()) {
//            object->ReplaceComponent<Transform>(panel->CurrentArea());
//            object->ReplaceComponent<Sizeable>(panel->CurrentArea());
//        }
    }
};


struct PanelAreaAlignSystem : public GameSystem<PanelArea, Transform, Sizeable> {
    
    void ObjectAdded(GameObject* object) override {
        object->Parent.Changed.Bind(this, &PanelAreaAlignSystem::ParentChanged, object);
        ParentChanged(object);
    }

    void ObjectRemoved(GameObject* object) override {
        object->Parent.Changed.Unbind(this, &PanelAreaAlignSystem::ParentChanged, object);
    }
    
    void ParentChanged(GameObject* object) {
        if (object->Parent.PreviousValue()) {
            object->Parent.PreviousValue()->GetComponent<Sizeable>()->Size.Changed.Unbind(this, &PanelAreaAlignSystem::ParentSizeChanged, object);
        }
        if (object->Parent() && object->Parent()->GetComponent<Sizeable>()) {
            object->Parent()->GetComponent<Sizeable>()->Size.Changed.Bind(this, &PanelAreaAlignSystem::ParentSizeChanged, object);
            ParentSizeChanged(object);
        }
    }
    
    void ParentSizeChanged(GameObject* object) {
        if (!object->Parent()) return;
        Sizeable* parentSizeable = object->Parent()->GetComponent<Sizeable>();
        
        PanelArea* area = object->GetComponent<PanelArea>();
        Transform* transform = object->GetComponent<Transform>();
        Sizeable* sizeable = object->GetComponent<Sizeable>();
        
        switch (area->Direction()) {
            case PanelArea::DirectionMode::Top: {
                transform->Position = {0,parentSizeable->Size().y*0.5f};
                sizeable->Size = { parentSizeable->Size().x, parentSizeable->Size().y*0.5f };
            }
            break;
            case PanelArea::DirectionMode::Bottom: {
                transform->Position = {0,0};
                sizeable->Size = { parentSizeable->Size().x, parentSizeable->Size().y*0.5f };
            }
            break;
            case PanelArea::DirectionMode::Left: {
                transform->Position = {0,0};
                sizeable->Size = { parentSizeable->Size().x*0.5f, parentSizeable->Size().y };
            }
            break;
            case PanelArea::DirectionMode::Right: {
                transform->Position = {parentSizeable->Size().x * 0.5f,0};
                sizeable->Size = { parentSizeable->Size().x*0.5f, parentSizeable->Size().y };
            }
            break;

          default:
            break;
        }
    }
};

struct PanelDropSystem : public GameSystem<Droppable, PanelDropper, Transform> {
    
    Gui* gui;
    
    void Initialize() override {
        gui = root->CreateSystem<Gui>();
    }

    void ObjectAdded(GameObject* object) override {
        object->GetComponent<Droppable>()->Dropped.Bind(this, &PanelDropSystem::Dropped);
    }

    void ObjectRemoved(GameObject* object) override {
        object->GetComponent<Droppable>()->Dropped.Unbind(this, &PanelDropSystem::Dropped);
    }
    
    GameObject* CreateArea(GameObject* parent, PanelArea::DirectionMode direction) {
        GameObject* object = gui->CreateControl(parent, "", 0, 0);
        object->RemoveComponent<Renderable>();
        PanelArea* area = object->AddComponent<PanelArea>();
        area->Direction = direction;
        return object;
    }
    
    void Dropped(DroppedData d) {
        GameObject* panelObject = d.object->GetComponent<PanelDropper>()->panel;
        Panel* panel = panelObject->GetComponent<Panel>();
        for(auto o : d.droppedTouches) {
            if (o.object == panelObject) continue;
            PanelArea* area = o.object->GetComponent<PanelArea>();
            if (!area) continue;
            Transform* transform = o.object->GetComponent<Transform>();
            if (!transform) continue;
            Sizeable* sizeable = o.object->GetComponent<Sizeable>();
            if (!sizeable) continue;
            if (panel->CurrentArea()==o.object) return;
            
            if (area->Panels.empty()) {
                panel->CurrentArea = o.object;
                area->Panels.push_back(panelObject);
                return;
            } else {
                Vector2 size = sizeable->Size;
                Vector2 localPosition = transform->WorldInverse().TransformPosition(o.WorldPosition);
                Vector2 top = { size.x * 0.5f, size.y };
                Vector2 bottom = { size.x * 0.5f, 0 };
                Vector2 left = { 0, size.y * 0.5f };
                Vector2 right = { size.x, size.y * 0.5f };
                
                Vector2 positions[] = { top, bottom, left, right };
                
                float minLength = 1000000.0f;
                int foundIndex = 0;
                for(int i=0; i<4; i++) {
                    Vector2 dist = localPosition - positions[i];
                    float len = dist.SquareLength();
                    if (len<minLength) {
                        minLength = len;
                        foundIndex = i;
                    }
                }
                
                GameObject* targetPanelArea = o.object;
                
                if (panel->CurrentArea()) {
                    auto& panels = panel->CurrentArea()->GetComponent<PanelArea>()->Panels;
                    auto it = std::find(panels.begin(), panels.end(), panelObject);
                    if (it!=panels.end()) {
                        panels.erase(it);
                    }
                    
                    if (panels.empty()) {
                        GameObject* parent = panel->CurrentArea()->Parent;
                    
                        GameObject* splitAreaObject = nullptr;
                        for(auto child : parent->Children()) {
                            if (child != panel->CurrentArea()) {
                                splitAreaObject = child;
                            }
                        }
                        panel->CurrentArea()->Remove();
                        
                        auto children = splitAreaObject->Children();
                        for(auto o : children) {
                            o->Parent = parent;
                        }
                        splitAreaObject->Remove();
                        
                        for(auto p : splitAreaObject->GetComponent<PanelArea>()->Panels) {
                            p->GetComponent<Panel>()->CurrentArea = parent;
                            parent->GetComponent<PanelArea>()->Panels.push_back(p);
                        }
                        splitAreaObject->GetComponent<PanelArea>()->Panels.clear();
                        
                        if (targetPanelArea == splitAreaObject) {
                            targetPanelArea = parent;
                        }
                    }
                }
                
                GameObject* target = nullptr;
                GameObject* splitArea = nullptr;
                
                auto panelsToMove = targetPanelArea->GetComponent<PanelArea>()->Panels;
                targetPanelArea->GetComponent<PanelArea>()->Panels.clear();
                
                if (foundIndex == 0) {
                    target = CreateArea(targetPanelArea, PanelArea::DirectionMode::Top);
                    splitArea = CreateArea(targetPanelArea, PanelArea::DirectionMode::Bottom);
                } else if (foundIndex == 1) {
                    target = CreateArea(targetPanelArea, PanelArea::DirectionMode::Bottom);
                    splitArea = CreateArea(targetPanelArea, PanelArea::DirectionMode::Top);
                } else if (foundIndex == 2) {
                    target = CreateArea(targetPanelArea, PanelArea::DirectionMode::Left);
                    splitArea = CreateArea(targetPanelArea, PanelArea::DirectionMode::Right);
                } else {
                    target = CreateArea(targetPanelArea, PanelArea::DirectionMode::Right);
                    splitArea = CreateArea(targetPanelArea, PanelArea::DirectionMode::Left);
                }
                
                panel->CurrentArea = target;
                target->GetComponent<PanelArea>()->Panels.push_back(panelObject);
                
                for(auto p : panelsToMove) {
                    p->GetComponent<Panel>()->CurrentArea = splitArea;
                    splitArea->GetComponent<PanelArea>()->Panels.push_back(p);
                }
            }
            
            return;
        }
    }
};


class TestWindowSystem : public GameState<TestWindowSystem> {
public:
    GameWorld world;
    GameObject* root;
    Gui* gui;
    
    void CreateArea(Vector2 size) {
        GameObject* object = gui->CreateControl(root, "", 0, size);
        object->AddComponent<PanelArea>();
        object->RemoveComponent<Renderable>();
    }
    
    void CreatePanel(Vector2 pos, Vector2 size, std::string text) {
        GameObject* window = gui->CreateControl(root, "TextBox", pos, size);
        window->AddComponent<Draggable>();
        window->AddComponent<Panel>();
        GameObject* bar = gui->CreateControl(window, "Box", {0, 0}, {size.x, 32});
        bar->AddComponent<Droppable>()->OnCreate = [this](GameObject* o, TouchData d) -> GameObject* {
            Vector3 position = o->GetComponent<Transform>()->World().TransformPosition(0);
            GameObject* control = gui->CreateControl(0, "Box", position, {200,25});
            return control;
        };
        bar->AddComponent<PanelDropper>()->panel = window;
        auto label = gui->CreateLabel(bar, 0, bar->GetComponent<Sizeable>()->Size, 0, text, 16);
        label->GetComponent<Label>()->HAlignment = Font::HAlignment::Center;
        label->GetComponent<Label>()->VAlignment = Font::VAlignment::Middle;
        label->GetComponent<Colorable>()->Color = Colour::Black();
    }
    
    void Initialize() {
    
        root = world.CreateRoot();
        gui = root->CreateSystem<Gui>();
        root->CreateSystem<PanelDropSystem>();
        root->CreateSystem<PanelSystem>();
        root->CreateSystem<PanelAreaAlignSystem>();
        
        gui->Setup("images.png", "images.xml", Context().Viewport());
        gui->CreateFont("/Library/Fonts/Arial Bold.ttf");//, "Font");
        
        CreateArea(Context().Viewport().Size());
        CreatePanel({0,50},{200,500}, "Project");
        CreatePanel({250,50},{200,500}, "Systems");
        CreatePanel({500,50},{200,500}, "Inspector");
        CreatePanel({560,50},{200,300}, "Extra");
        CreatePanel({600,50},{200,300}, "Animation");
        
    }
    
    void Update(float dt) {
        Context().InputDevice().UpdateInputManager(&world.Input());
        world.Update(dt);
    }
    
    void Render() {
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<TestWindowSystem>();
	return 0;
}
