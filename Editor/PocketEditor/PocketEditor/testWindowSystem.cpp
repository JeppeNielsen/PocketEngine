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

enum class PanelDirection { Top, Bottom, Left, Right };

class PanelLocation {
    std::vector<PanelDirection> directions;
    
public:
    
    inline bool operator !=(const PanelLocation &other) const{
        if (other.directions.size()!=directions.size()) return true;
        for(int i=0; i<directions.size(); ++i) {
            if (directions[i]!=other.directions[i]) return true;
        }
        return false;
    }

    inline bool operator == (const PanelLocation &other) const{
        return !operator!=(other);
    }
    
    void Clear() {
        directions.clear();
    }
    
    void Push(PanelDirection direction) {
        directions.push_back(direction);
    }
    
    bool StartsWith(const PanelLocation& location) const {
        if (location.directions.size()>=directions.size()) return false;
        for(int i=0; i<location.directions.size(); ++i) {
            if (location.directions[i]!=directions[i]) return false;
        }
        return true;
    }
    
    void RemoveSplit(const PanelLocation& location) {
        if (directions.empty()) return;
        if (location.directions.size()>directions.size()) return;
        directions.erase(directions.begin() + location.directions.size() - 1);
    }
    
    PanelLocation Split() const {
        if (directions.empty()) return PanelLocation();
        PanelLocation split = *this;
        split.directions.pop_back();
        return split;
    }
    
    std::string Id() {
        const char dirIds[] = "TBLR";
        std::string id = "";
        for(auto d : directions) {
            id += dirIds[(int)d];
        }
        return id;
    }
    
    Rect GetRect(Vector2 size, const std::function<float(const std::string&)>& splitFunction) {
        const char dirIds[] = "TBLR";
        Rect rect(0, size);
        std::string id = "";
        for(auto d : directions) {
            rect = DivideRect(rect, d, splitFunction ? splitFunction(id) : 0.5f);
            id += dirIds[(int)d];
        }
        return rect;
    }
    
    Rect DivideRect(Rect input, PanelDirection direction, float splitPosition) {
        
        switch (direction) {
            case PanelDirection::Top: {
                input.y += input.height * splitPosition;
                input.height *= (1 - splitPosition);
            }
            break;
            case PanelDirection::Bottom: {
                input.height *= splitPosition;
            }
            break;
            case PanelDirection::Left: {
                input.width *= splitPosition;
            }
            break;
            case PanelDirection::Right: {
                input.x += input.width * splitPosition;
                input.width *= (1 - splitPosition);
            }
            break;

          default:
            break;
        }

        return input;
    }
    
    bool IsHorizontal() {
        return directions.empty() ? false : (directions.back() == PanelDirection::Right || directions.back() == PanelDirection::Left);
    }
};


class Panel {
public:
    Property<GameObject*> Area;

    Panel() {
        Area = nullptr;
    }
    
    PanelLocation location;
    
    Event<> Dirty;

private:
};


struct PanelArea {
    PanelArea() { IsDirty = false; }
    std::map<std::string, float> Splits;
    
    float GetSplitValue(const std::string& id) {
        auto it = Splits.find(id);
        return it!=Splits.end() ? it->second : 0.5f;
    }
    
    void SetSplitValue(const std::string& id, float value) {
        Splits[id] = value;
        SplitValueChanged(id);
    }
    
    bool IsDirty;
    
    Event<std::string> SplitValueChanged;
};

struct PanelSplitter {
    GameObject* area;
    PanelLocation location;
    bool isHorizontal;
};


struct PanelDropper {
    GameObject* panel;
};

struct PanelSplitterSystem : public GameSystem<PanelSplitter, Draggable, Transform> {

    std::set<GameObject*> splittersNeedingAlignment;

    void ObjectAdded(GameObject* object) override {
        object->GetComponent<PanelSplitter>()->area->GetComponent<Sizeable>()->Size.Changed.Bind(this, &PanelSplitterSystem::AreaSizeChanged, object);
        object->GetComponent<PanelSplitter>()->area->GetComponent<PanelArea>()->SplitValueChanged.Bind(this, &PanelSplitterSystem::SplitValueChanged, object);
        splittersNeedingAlignment.insert(object);
    }
    
    void ObjectRemoved(GameObject* object) override {
        object->GetComponent<PanelSplitter>()->area->GetComponent<Sizeable>()->Size.Changed.Unbind(this, &PanelSplitterSystem::AreaSizeChanged, object);
        object->GetComponent<PanelSplitter>()->area->GetComponent<PanelArea>()->SplitValueChanged.Unbind(this, &PanelSplitterSystem::SplitValueChanged, object);
        auto it = splittersNeedingAlignment.find(object);
        if (it!=splittersNeedingAlignment.end()) {
            splittersNeedingAlignment.erase(it);
        }
    }

    void AreaSizeChanged(GameObject* object) {
        splittersNeedingAlignment.insert(object);
    }
    
    void SplitValueChanged(std::string id, GameObject* object) {
        splittersNeedingAlignment.insert(object);
    }

    void Update(float dt) override {
        for(auto o : Objects()) {
            if (o->GetComponent<Draggable>()->IsDragging) {
                SetSplitValueFromTransform(o);
            }
        }
    
        for(auto o : splittersNeedingAlignment) {
            AlignSplitter(o);
        }
        splittersNeedingAlignment.clear();
    }
    
    void AlignSplitter(GameObject* object) {
        PanelSplitter* splitter = object->GetComponent<PanelSplitter>();
        Sizeable* areaSize = splitter->area->GetComponent<Sizeable>();
        PanelArea* area = splitter->area->GetComponent<PanelArea>();
        Transform* transform = object->GetComponent<Transform>();
        Sizeable* sizeable = object->GetComponent<Sizeable>();
        
        float splitValue = area->GetSplitValue(splitter->location.Id());
        
        Rect rect = splitter->location.GetRect(areaSize->Size, [area] (const std::string& id) {
           return area->GetSplitValue(id);
        });
        
        if (splitter->isHorizontal) {
            transform->Position = { rect.x + rect.width * splitValue-5, rect.y };
            sizeable->Size = { 10, rect.height };
        } else {
            transform->Position = { rect.x,  rect.y + rect.height * splitValue-5 };
            sizeable->Size = { rect.width, 10 };
        }
    }
    
    void SetSplitValueFromTransform(GameObject* object) {
        PanelSplitter* splitter = object->GetComponent<PanelSplitter>();
        Sizeable* areaSize = splitter->area->GetComponent<Sizeable>();
        PanelArea* area = splitter->area->GetComponent<PanelArea>();
        Transform* transform = object->GetComponent<Transform>();
        
        Rect rect = splitter->location.GetRect(areaSize->Size, [area] (const std::string& id) {
           return area->GetSplitValue(id);
        });
        
        float splitValue;
        
        if (splitter->isHorizontal) {
            splitValue = (transform->Position().x - rect.x + 5) / rect.width;
        } else {
            splitValue = (transform->Position().y - rect.y + 5) / rect.height;
        }
        
        area->SetSplitValue(splitter->location.Id(), splitValue);
    }

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
            prev->GetComponent<PanelArea>()->SplitValueChanged.Unbind(this, &PanelSystem::SplitValueChanged, object);
        }
        if (current) {
            current->GetComponent<Sizeable>()->Size.Changed.Bind(this, &PanelSystem::AreaSizeChanged, object);
            current->GetComponent<PanelArea>()->SplitValueChanged.Bind(this, &PanelSystem::SplitValueChanged, object);
            AreaSizeChanged(object);
        }
    }
    
    void AreaSizeChanged(GameObject* object) {
        Panel* panel = object->GetComponent<Panel>();
        GameObject* area = panel->Area;
        if (!area) return;
        
        PanelArea* panelArea = area->GetComponent<PanelArea>();
        
        object->Parent = area;
        
        Sizeable* areaSizeable = area->GetComponent<Sizeable>();
        Rect rect = panel->location.GetRect(areaSizeable->Size(), [panelArea] (const std::string& id) {
            return panelArea->GetSplitValue(id);
        });
        object->GetComponent<Transform>()->Position = rect.Position();
        object->GetComponent<Sizeable>()->Size = rect.Size();
    }
    
    void SplitValueChanged(std::string id, GameObject* object) {
        AreaSizeChanged(object);
    }
};

struct PanelAreaSystem : public GameSystem<PanelArea> {

    PanelSystem* panels;
    Gui* gui;

    void Initialize() override {
        panels = root->CreateSystem<PanelSystem>();
        gui = root->CreateSystem<Gui>();
    }

    void ObjectAdded(GameObject* object) override {
        
    }
    
    void ObjectRemoved(GameObject* object) override {
    
    }
    
    void Update(float dt) override {
        for(auto o : Objects()) {
            PanelArea* area = o->GetComponent<PanelArea>();
            if (area->IsDirty) {
                area->IsDirty = false;
                CreateSplitters(o, area);
            }
        }
    }
    
    void CreateSplitters(GameObject* object, PanelArea* area) {
        for(auto o : object->Children()) {
            if (o->GetComponent<PanelSplitter>()) {
                o->Remove();
            }
        }
        
        std::map<std::string, float> newSplits;
        
        for(auto o : panels->Objects()) {
            Panel* panel = o->GetComponent<Panel>();
            GameObject* panelArea = panel->Area;
            if (panelArea!=object) continue;
            
            auto splitLocation = panel->location.Split();
            std::string id = splitLocation.Id();
            
            if (newSplits.find(id)!=newSplits.end()) continue;
            
            auto it = area->Splits.find(id);
            if (it != area->Splits.end()) {
                newSplits[id] = it->second;
            } else {
                newSplits[id] = 0.5f;
            }
            
            GameObject* splitter = gui->CreateControl(object, "Box");
            splitter->AddComponent<PanelSplitter>()->location = splitLocation;
            splitter->GetComponent<PanelSplitter>()->isHorizontal = panel->location.IsHorizontal();
            splitter->GetComponent<PanelSplitter>()->area = panelArea;
            splitter->AddComponent<Draggable>()->Movement = panel->location.IsHorizontal() ? Draggable::MovementMode::XAxis : Draggable::MovementMode::YAxis;
        }
        
        area->Splits = newSplits;
        
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
            
            PanelArea* panelArea = droppedArea->GetComponent<PanelArea>();
            
            Vector2 localPosition = areaTransform->WorldInverse().TransformPosition(o.WorldPosition);
            Rect rect = droppedPanel->location.GetRect(areaSizable->Size, [panelArea] (const std::string& id) {
                return panelArea->GetSplitValue(id);
            });
            
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
            
            //if (!panel->Directions.empty()) {
                for(auto o : panels->Objects()) {
                    if (o == panelObject) continue;
                    auto op = o->GetComponent<Panel>();
                    if (!op) continue;
                    if (op->Area != panel->Area) continue;
                    
                    if (op->location.StartsWith(panel->location.Split())) {
                        op->location.RemoveSplit(panel->location);
                        op->Dirty();
                    }
                }
            //}
            
            panel->location = droppedPanel->location;
            
            if (foundIndex == 0) {
                panel->location.Push(PanelDirection::Top);
                droppedPanel->location.Push(PanelDirection::Bottom);
            } else if ( foundIndex == 1) {
                panel->location.Push(PanelDirection::Bottom);
                droppedPanel->location.Push(PanelDirection::Top);
            } else if (foundIndex == 2) {
                panel->location.Push(PanelDirection::Left);
                droppedPanel->location.Push(PanelDirection::Right);
            } else if (foundIndex == 3) {
                panel->location.Push(PanelDirection::Right);
                droppedPanel->location.Push(PanelDirection::Left);
            }
            
            panelArea->IsDirty = true;
            
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
        root->CreateSystem<PanelAreaSystem>();
        root->CreateSystem<PanelSplitterSystem>();
        
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
