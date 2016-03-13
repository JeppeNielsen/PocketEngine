//
//  SelectableDragSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/26/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "SelectableDragSystem.hpp"

using namespace Pocket;

void SelectableDragSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<Draggable>()->IsDragging.Changed.Bind(this, &SelectableDragSystem::IsDraggingChanged, object);
}

void SelectableDragSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Draggable>()->IsDragging.Changed.Unbind(this, &SelectableDragSystem::IsDraggingChanged, object);
    auto it = draggingObjects.find(object);
    if (it!=draggingObjects.end()) draggingObjects.erase(it);
}

void SelectableDragSystem::IsDraggingChanged(GameObject* object) {
    Draggable* draggable = object->GetComponent<Draggable>();
    
    if (draggable->IsDragging) {
        if(!object->GetComponent<Selectable>()->Selected) return;
        DragData& data = draggingObjects[object];
        Vector3 position = object->GetComponent<Transform>()->World().TransformPosition(0);
        
        for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
            GameObject* go = *it;
            if (go == object) continue;
            if (!go->GetComponent<Selectable>()->Selected) continue;
            data.objects.push_back(go);
            data.offsets.push_back(position - go->GetComponent<Transform>()->World().TransformPosition(0));
        }
    } else {
        DraggingObjects::iterator it = draggingObjects.find(object);
        if (it!=draggingObjects.end()) draggingObjects.erase(it);
    }
}

void SelectableDragSystem::Update(float dt) {
    for (DraggingObjects::iterator it = draggingObjects.begin(); it!=draggingObjects.end(); ++it) {
        Vector3 position = it->first->GetComponent<Transform>()->World().TransformPosition(0);
        DragData& data = it->second;
        for (int i=0; i<data.objects.size(); i++) {
            Transform* transform = data.objects[i]->GetComponent<Transform>();
            Vector3 inversePosition = transform->WorldInverse().TransformPosition(position - data.offsets[i]);

            Vector3 localPosition = transform->Local().TransformPosition(inversePosition+transform->Anchor());
            
            data.objects[i]->GetComponent<Transform>()->Position = localPosition;
        }
    }
}