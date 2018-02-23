//
//  DroppableSystem.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "DroppableSystem.hpp"
#include "Draggable.hpp"
#include "Mesh.hpp"

using namespace Pocket;

void DroppableSystem::CreateSubSystems(Pocket::GameStorage &storage) {
    storage.AddSystemType<TouchSystem>();
}

void DroppableSystem::Initialize() {
    touchSystem = root->CreateSystem<TouchSystem>();
}

void DroppableSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<Touchable>()->Down.Bind(this, &DroppableSystem::TouchDown, object);
    object->GetComponent<Touchable>()->Up.Bind(this, &DroppableSystem::TouchUp, object);
}

void DroppableSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Touchable>()->Down.Unbind(this, &DroppableSystem::TouchDown, object);
    object->GetComponent<Touchable>()->Up.Unbind(this, &DroppableSystem::TouchUp, object);
    auto it = downObjects.find(object);
    if (it!=downObjects.end()) {
        downObjects.erase(it);
    }
}

void DroppableSystem::Update(float dt) {
    for(auto it = downObjects.begin(); it!=downObjects.end();) {
        Vector2 pos = root->Input().GetTouchPosition(it->second.touchData.Index);
        Vector2 delta = pos - it->second.prevPosition;
        if (delta.Length()>it->second.maxMovement) {
            DropStarted(it->second.touchData, it->first);
            it = downObjects.erase(it);
        } else {
            ++it;
        }
    }
}

void DroppableSystem::TouchDown(Pocket::TouchData d, Pocket::GameObject *object) {
    DownObject& downObject = downObjects[object];
    downObject.touchData = d;
    downObject.prevPosition = d.Position;
    downObject.maxMovement =  object->GetComponent<Droppable>()->activateThreshhold;
    downObject.createdObject = 0;
}

void DroppableSystem::DropStarted(Pocket::TouchData d, Pocket::GameObject *object) {
    Droppable* droppable = object->GetComponent<Droppable>();
    GameObject* clone = droppable->OnCreate(object, d);
    clone->AddComponent<Draggable>();
    touchSystem->EnqueueDown(clone, d);
    SetMetaData(object, clone);
}

void DroppableSystem::TouchUp(Pocket::TouchData d, GameObject* object) {

    auto it = downObjects.find(object);
    if (it!=downObjects.end()) {
        downObjects.erase(it);
    } else {

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
        GameObject* clone = (GameObject*)GetMetaData(object);
        clone->Remove();
    }
}
