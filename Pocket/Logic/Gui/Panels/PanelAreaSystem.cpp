//
//  PanelAreaSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "PanelAreaSystem.hpp"
#include "PanelSystem.hpp"
#include "Gui.hpp"

using namespace Pocket;

void PanelAreaSystem::Initialize() {
    root->CreateSystem<PanelSplitterSystem>()->Order = -1;
    panels = root->CreateSystem<PanelSystem>();
    gui = root->CreateSystem<class Gui>();
}

void PanelAreaSystem::CreateSubSystems(GameStorage& storage) {
    storage.AddSystemType<PanelSplitterSystem>();
    storage.AddSystemType<PanelSystem>();
    storage.AddSystemType<Gui>();
}

void PanelAreaSystem::Update(float dt) {
    for(auto o : Objects()) {
        PanelArea* area = o->GetComponent<PanelArea>();
        if (area->IsDirty) {
            area->IsDirty = false;
            CreateSplitters(o, area);
        }
    }
}

void PanelAreaSystem::CreateSplitters(GameObject* object, PanelArea* area) {
    for(auto o : object->Hierarchy().Children()) {
        if (o->GetComponent<PanelSplitter>()) {
            o->Remove();
        }
    }
    
    std::map<std::string, float> newSplits;
    
    for(auto o : panels->Objects()) {
        Panel* panel = o->GetComponent<Panel>();
        GameObject* panelArea = panel->Area;
        if (panelArea!=object) continue;
        
        panel->location.RecurseLocations([this, &newSplits, area, object, panelArea, panel] (const PanelLocation& location) {
            PanelLocation splitLocation = location.Split();
            std::string id = splitLocation.Id();
            
            if (newSplits.find(id)!=newSplits.end()) return;
            
            auto it = area->Splits.find(id);
            if (it != area->Splits.end()) {
                newSplits[id] = it->second;
            } else {
                newSplits[id] = 0.5f;
            }
            
            GameObject* splitter = gui->CreateControl(object, "Box");
            splitter->AddComponent<PanelSplitter>()->location = splitLocation;
            splitter->GetComponent<PanelSplitter>()->isHorizontal = location.IsHorizontal();
            splitter->GetComponent<PanelSplitter>()->area = panelArea;
            splitter->AddComponent<Draggable>()->Movement = location.IsHorizontal() ? Draggable::MovementMode::XAxis : Draggable::MovementMode::YAxis;
            splitter->RemoveComponent<Renderable>();
        });
        
        
    }
    
    area->Splits = newSplits;
}

void PanelAreaSystem::PanelSplitterSystem::ObjectAdded(GameObject* object) {
    object->GetComponent<PanelSplitter>()->area->GetComponent<Sizeable>()->Size.Changed.Bind(this, &PanelSplitterSystem::AreaSizeChanged, object);
    object->GetComponent<PanelSplitter>()->area->GetComponent<PanelArea>()->SplitValueChanged.Bind(this, &PanelSplitterSystem::SplitValueChanged, object);
    splittersNeedingAlignment.insert(object);
}

void PanelAreaSystem::PanelSplitterSystem::ObjectRemoved(GameObject* object) {
    object->GetComponent<PanelSplitter>()->area->GetComponent<Sizeable>()->Size.Changed.Unbind(this, &PanelSplitterSystem::AreaSizeChanged, object);
    object->GetComponent<PanelSplitter>()->area->GetComponent<PanelArea>()->SplitValueChanged.Unbind(this, &PanelSplitterSystem::SplitValueChanged, object);
    auto it = splittersNeedingAlignment.find(object);
    if (it!=splittersNeedingAlignment.end()) {
        splittersNeedingAlignment.erase(it);
    }
}

void PanelAreaSystem::PanelSplitterSystem::AreaSizeChanged(GameObject* object) {
    splittersNeedingAlignment.insert(object);
}

void PanelAreaSystem::PanelSplitterSystem::SplitValueChanged(std::string id, GameObject* object) {
    splittersNeedingAlignment.insert(object);
}

void PanelAreaSystem::PanelSplitterSystem::Update(float dt) {
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

void PanelAreaSystem::PanelSplitterSystem::AlignSplitter(GameObject* object) {
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

void PanelAreaSystem::PanelSplitterSystem::SetSplitValueFromTransform(GameObject* object) {
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
