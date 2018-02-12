//
//  PanelSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "PanelSystem.hpp"
#include "PanelArea.hpp"

using namespace Pocket;

void PanelSystem::ObjectAdded(GameObject* object) {
    Panel* panel = object->GetComponent<Panel>();
    panel->Area.Changed.Bind(this, &PanelSystem::AreaChanged, object);
    InvokeAreaChanged(object, nullptr, panel->Area);
    panel->Dirty.Bind(this, &PanelSystem::AreaSizeChanged, object);
    AreaSizeChanged(object);
}

void PanelSystem::ObjectRemoved(GameObject* object) {
    Panel* panel = object->GetComponent<Panel>();
    panel->Area.Changed.Unbind(this, &PanelSystem::AreaChanged, object);
    InvokeAreaChanged(object, panel->Area, nullptr);
    panel->Dirty.Unbind(this, &PanelSystem::AreaSizeChanged, object);
}

void PanelSystem::AreaChanged(GameObject* object) {
    Panel* panel = object->GetComponent<Panel>();
    InvokeAreaChanged(object, panel->Area.PreviousValue(), panel->Area);
}

void PanelSystem::InvokeAreaChanged(GameObject* object, GameObject* prev, GameObject* current) {
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

void PanelSystem::AreaSizeChanged(GameObject* object) {
    Panel* panel = object->GetComponent<Panel>();
    GameObject* area = panel->Area;
    if (!area) return;
    
    PanelArea* panelArea = area->GetComponent<PanelArea>();
    
    object->Hierarchy().Parent = area;
    
    Sizeable* areaSizeable = area->GetComponent<Sizeable>();
    Rect rect = panel->location.GetRect(areaSizeable->Size(), [panelArea] (const std::string& id) {
        return panelArea->GetSplitValue(id);
    });
    object->GetComponent<Transform>()->Position = rect.Position();
    object->GetComponent<Sizeable>()->Size = rect.Size();
}

void PanelSystem::SplitValueChanged(std::string id, GameObject* object) {
    AreaSizeChanged(object);
}
