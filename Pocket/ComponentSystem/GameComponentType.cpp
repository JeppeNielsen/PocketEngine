//
//  GameComponentType.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 9/29/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "GameComponentType.hpp"
#include "GameObject.hpp"
#include "GameSystem.hpp"
#include "GameComponent.hpp"

using namespace Pocket;

IGameComponentType::IGameComponentType() : isInEnabledChangedList(false) {}

IGameComponentType::~IGameComponentType() {}

int GameComponentTypeFactory::componentIdCounter = 0;

GameComponentTypeFactory::GameComponentTypes* GameComponentTypeFactory::createdComponentTypes = 0;

IGameComponentType* GameComponentTypeFactory::CreateType(int index) {
    return createdComponentTypes->at(index)->Clone();
}

void IGameComponentType::UpdateChangedObjects() {
    isInEnabledChangedList = false;
    ObjectCollection temp = changedObjects;
    changedObjects.clear();
    for (size_t i=0; i<temp.size(); i++) {
        GameObject* object = temp[i];
        bool wantsEnable = (object->enabledComponents & mask) == mask;
        bool isActive = (object->activeComponents & mask) == mask;
        if (wantsEnable == isActive) continue;
        UpdateChangedObject(object, wantsEnable);
    }
}

void IGameComponentType::UpdateChangedObject(GameObject* object, bool enable) {
    
    if (enable) {
        object->activeComponents |= mask;
        for (size_t i=0; i<systems.size(); i++) {
            GameSystem* system = systems[i];
            bool isInterest = (object->activeComponents & system->aspect) == system->aspect;
            if (isInterest) {
                object->systemIndicies[system->indexInList] = (unsigned short)system->objects.size();
				system->objects.push_back(object);
				system->ObjectAdded(object);
			}
        }
    } else {
        for (size_t i=0; i<systems.size(); i++) {
            GameSystem* system = systems[i];
            bool wasInterest = (object->activeComponents & system->aspect) == system->aspect;
            if (wasInterest) {
                system->ObjectRemoved(object);
				unsigned short index = object->systemIndicies[system->indexInList];
    			GameObject* last = system->objects.back();
                system->objects[index] = last;
				last->systemIndicies[system->indexInList] = index;
				system->objects.pop_back();
            }
        }
        object->activeComponents &= maskInverse;
    }
}

void IGameComponentType::UpdateRemovedObjects() {
    isInRemovedList = false;
    RemovedObjects temp = removedObjects;
    removedObjects.clear();
    
    for (RemovedObjects::iterator it = temp.begin(); it!=temp.end(); ++it) {
        UpdateRemovedObject(*it);
    }
}
