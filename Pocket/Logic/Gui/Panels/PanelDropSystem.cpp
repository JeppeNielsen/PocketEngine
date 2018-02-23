//
//  PanelDropSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 04/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "PanelDropSystem.hpp"
#include "PanelSystem.hpp"
#include "PanelArea.hpp"

using namespace Pocket;

void PanelDropSystem::CreateSubSystems(Pocket::GameStorage& storage) {
    storage.AddSystemType<PanelSystem>();
}

void PanelDropSystem::Initialize() {
    panels = root->CreateSystem<PanelSystem>();
}

void PanelDropSystem::ObjectAdded(GameObject* object) {
    object->GetComponent<Droppable>()->Dropped.Bind(this, &PanelDropSystem::Dropped);
}

void PanelDropSystem::ObjectRemoved(GameObject* object) {
    object->GetComponent<Droppable>()->Dropped.Unbind(this, &PanelDropSystem::Dropped);
}

void PanelDropSystem::Dropped(DroppedData d) {
    GameObject* panelObject = d.object->GetComponent<PanelDropper>()->panel;
    Panel* panel = panelObject->GetComponent<Panel>();
    for(auto o : d.droppedTouches) {
        if (o.object == panelObject) continue;
        
        PanelArea* droppedPanelArea = o.object->GetComponent<PanelArea>();
        if (droppedPanelArea) {
            panel->Area = o.object;
            return;
        }
        
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
