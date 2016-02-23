//
//  DroppableSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Droppable.hpp"
#include "DroppableSystem.hpp"
#include "TouchSystem.hpp"

namespace Pocket {
    template<typename T>
    struct DroppableSystem : public GameSystem<T, Droppable, Touchable> {
    
        using GameObject = GameObject<T>;
        using Systems = meta::list<TouchSystem>;
    
        void Initialize() {
            touchSystem = this->World().GetSystem<TouchSystem>();
        }
    
        void ObjectAdded(GameObject *object) {
            object->GetComponent<Touchable>()->Up += event_handler(this, &DroppableSystem::TouchUp, object);
        }

        void ObjectRemoved(GameObject *object) {
            object->GetComponent<Touchable>()->Up -= event_handler(this, &DroppableSystem::TouchUp, object);
        }

        void TouchUp(Pocket::TouchData d, GameObject* object) {
            DroppedData droppedData;
            droppedData.object = object;
            droppedData.touchData = d;
            TouchEvent e(d.Index, d.Input->GetTouchPosition(d.Index));
            touchSystem->FindTouchedObjects(droppedData.droppedTouches, e, true);
            for (int i=0; i<droppedData.droppedTouches.size(); ++i) {
                if (droppedData.droppedTouches[i].object == object) {
                    droppedData.droppedTouches.erase(droppedData.droppedTouches.begin()+i);
                    break;
                }
            }
            object->GetComponent<Droppable>()->Dropped(droppedData);
        }

        TouchSystem* touchSystem;
    };
}