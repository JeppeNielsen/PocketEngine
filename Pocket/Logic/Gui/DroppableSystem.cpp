//
//  DroppableSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "DroppableSystem.hpp"

using namespace Pocket;

void DroppableSystem::Initialize(GameWorld* world) {
    touchSystem = world->CreateSystem<TouchSystem>();
}

void DroppableSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<Touchable>()->Up.Bind(this, &DroppableSystem::TouchUp, object);
}

void DroppableSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Touchable>()->Up.Unbind(this, &DroppableSystem::TouchUp, object);
}

void DroppableSystem::TouchUp(Pocket::TouchData d, GameObject* object) {
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
