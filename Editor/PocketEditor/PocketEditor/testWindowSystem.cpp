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
        IsDirty = false;
    }
    bool IsDirty;
};

class Panel {
public:
    Property<GameObject*> Area;

    Panel() {
        Area = nullptr;
    }
    
    enum class Direction { Top, Bottom, Left, Right };
    
    std::vector<Direction> Directions;
    
    Rect FindRectFromPath(Vector2 size) {
        Rect rect(0, size);
        for(auto d : Directions) {
            rect = DivideRect(rect, d);
        }
        return rect;
    }
    
    Event<> Dirty;

private:
    Rect DivideRect(Rect input, Panel::Direction direction) {
        
        switch (direction) {
            case Panel::Direction::Top: {
                input.height *= 0.5f;
                input.y += input.height;
            }
            break;
            case Panel::Direction::Bottom: {
                input.height *= 0.5f;
            }
            break;
            case Panel::Direction::Left: {
                input.width *= 0.5f;
            }
            break;
            case Panel::Direction::Right: {
                input.width *= 0.5f;
                input.x += input.width;
            }
            break;

          default:
            break;
        }

        return input;
    }
};

struct PanelDropper {
    GameObject* panel;
};

struct PanelSystem : public GameSystem<Panel, Transform, Sizeable> {

    void ObjectAdded(GameObject* object) override {
        Panel* panel = object->GetComponent<Panel>();
        panel->Area.Changed.Bind(this, &PanelSystem::AreaChanged, object);
        InvokeAreaChanged(object, nullptr, panel->Area);
        panel->Dirty.Bind(this, &PanelSystem::AreaSizeChanged, object);
        AreaSizeChanged(object);
    }

    void ObjectRemoved(GameObject* object) override {
        Panel* panel = object->GetComponent<Panel>();
        panel->Area.Changed.Unbind(this, &PanelSystem::AreaChanged, object);
        InvokeAreaChanged(object, panel->Area, nullptr);
        panel->Dirty.Unbind(this, &PanelSystem::AreaSizeChanged, object);
    }
    
    void AreaChanged(GameObject* object) {
        Panel* panel = object->GetComponent<Panel>();
        InvokeAreaChanged(object, panel->Area.PreviousValue(), panel->Area);
    }
    
    void InvokeAreaChanged(GameObject* object, GameObject* prev, GameObject* current) {
        if (prev) {
            prev->GetComponent<Sizeable>()->Size.Changed.Unbind(this, &PanelSystem::AreaSizeChanged, object);
        }
        if (current) {
            current->GetComponent<Sizeable>()->Size.Changed.Bind(this, &PanelSystem::AreaSizeChanged, object);
            AreaSizeChanged(object);
        }
    }
    
    void AreaSizeChanged(GameObject* object) {
        Panel* panel = object->GetComponent<Panel>();
        GameObject* area = panel->Area;
        if (!area) return;
        
        object->Parent = area;
        
        Sizeable* areaSizeable = area->GetComponent<Sizeable>();
        Rect rect = panel->FindRectFromPath(areaSizeable->Size());
        object->GetComponent<Transform>()->Position = rect.Position();
        object->GetComponent<Sizeable>()->Size = rect.Size();
    }
    
};

struct PanelDropSystem : public GameSystem<Droppable, PanelDropper, Transform> {
    
    PanelSystem* panels;
    
    void Initialize() override {
        panels = root->CreateSystem<PanelSystem>();
    }
    
    void ObjectAdded(GameObject* object) override {
        object->GetComponent<Droppable>()->Dropped.Bind(this, &PanelDropSystem::Dropped);
    }

    void ObjectRemoved(GameObject* object) override {
        object->GetComponent<Droppable>()->Dropped.Unbind(this, &PanelDropSystem::Dropped);
    }
    
    void Dropped(DroppedData d) {
        GameObject* panelObject = d.object->GetComponent<PanelDropper>()->panel;
        Panel* panel = panelObject->GetComponent<Panel>();
        for(auto o : d.droppedTouches) {
            if (o.object == panelObject) continue;
            
            Panel* droppedPanel = o.object->GetComponent<Panel>();
            if (!droppedPanel) continue;
            
            GameObject* droppedArea = droppedPanel->Area;
            if (!droppedArea) continue;
            
            Sizeable* areaSizable = droppedArea->GetComponent<Sizeable>();
            if (!areaSizable) continue;
            
            Transform* areaTransform = droppedArea->GetComponent<Transform>();
            if (!areaTransform) continue;
            
            
            Vector2 localPosition = areaTransform->WorldInverse().TransformPosition(o.WorldPosition);
            Rect rect = droppedPanel->FindRectFromPath(areaSizable->Size);
            
            localPosition -= rect.Position();
            
            Vector2 positions[] = {
                { rect.width * 0.5f, rect.height },
                { rect.width * 0.5f, 0 },
                { 0, rect.height*0.5f },
                { rect.width, rect.height*0.5f }
            };
            
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
            
            if (!panel->Directions.empty()) {
                for(auto o : panels->Objects()) {
                    if (o == panelObject) continue;
                    auto op = o->GetComponent<Panel>();
                    if (!op) continue;
                    if (op->Area != panel->Area) continue;
                    if (op->Directions.size() < panel->Directions.size()) continue;
                    
                    bool isSame = true;
                    for(int i = 0; i < panel->Directions.size() - 1; i++ ) {
                        if (panel->Directions[i] != op->Directions[i]) {
                            isSame = false;
                            break;
                        }
                    }
                    
                    if (isSame) {
                        op->Directions.erase(op->Directions.begin() + panel->Directions.size() - 1);
                        op->Dirty();
                    }
                }
            }
            
            panel->Directions = droppedPanel->Directions;
            
            if (foundIndex == 0) {
                panel->Directions.push_back(Panel::Direction::Top);
                droppedPanel->Directions.push_back(Panel::Direction::Bottom);
            } else if ( foundIndex == 1) {
                panel->Directions.push_back(Panel::Direction::Bottom);
                droppedPanel->Directions.push_back(Panel::Direction::Top);
            } else if (foundIndex == 2) {
                panel->Directions.push_back(Panel::Direction::Left);
                droppedPanel->Directions.push_back(Panel::Direction::Right);
            } else if (foundIndex == 3) {
                panel->Directions.push_back(Panel::Direction::Right);
                droppedPanel->Directions.push_back(Panel::Direction::Left);
            }
            
            panel->Dirty();
            droppedPanel->Dirty();
            
            panel->Area = droppedArea;
            
            return;
        }
    }
};


class TestWindowSystem : public GameState<TestWindowSystem> {
public:
    GameWorld world;
    GameObject* root;
    Gui* gui;
    GameObject* area;
    
    GameObject* CreateArea(Vector2 size) {
        GameObject* object = gui->CreateControl(root, "", 0, size);
        object->AddComponent<PanelArea>();
        object->RemoveComponent<Renderable>();
        return object;
    }
    
    Panel* CreatePanel(Vector2 pos, Vector2 size, std::string text) {
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
        
        return window->GetComponent<Panel>();
    }
    
    void Initialize() {
    
        root = world.CreateRoot();
        gui = root->CreateSystem<Gui>();
        root->CreateSystem<PanelDropSystem>();
        root->CreateSystem<PanelSystem>();
        
        gui->Setup("images.png", "images.xml", Context().Viewport());
        gui->CreateFont("/Library/Fonts/Arial Bold.ttf");//, "Font");
        
        area = CreateArea(Context().Viewport().Size());
        
        auto p = CreatePanel({0,50},{200,500}, "Project");
        p->Area = area;
        
        CreatePanel({250,50},{200,500}, "Systems");
        CreatePanel({500,50},{200,500}, "Inspector");
        CreatePanel({560,50},{200,300}, "Extra");
        CreatePanel({600,50},{200,300}, "Animation");
        
    }
    
    void Update(float dt) {
        area->GetComponent<Sizeable>()->Size = Context().ScreenSize;
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
