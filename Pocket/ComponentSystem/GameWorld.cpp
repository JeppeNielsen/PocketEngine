//
//  GameWorld.cpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 06/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameWorld.hpp"
#include <iostream>

using namespace Pocket;

GameWorld::GameWorld() {
    for(int i=0; i<MaxComponents; ++i) {
        components[i] = 0;
    }
    root.world = this;
    objectCount = 0;
}

GameWorld::~GameWorld() {
    Clear();
    for(auto s : systems) {
        delete s;
    }
    for(int i=0; i<MaxComponents; ++i) {
        delete components[i];
    }
}

const GameObject* GameWorld::Root() { return &root; }

GameObject* GameWorld::CreateObject() {
    int index;
    if (objectsFreeIndicies.empty()) {
        index = (int)objects.size();
        objects.resize(index + 1);
        if (index>=objectComponents[0].size()) {
            for(int i=0; i<MaxComponents; i++) {
                objectComponents[i].resize(index + 32);
            }
        }
    } else {
        index = objectsFreeIndicies.back();
        objectsFreeIndicies.pop_back();
    }
    
    for(int i=0; i<MaxComponents; i++) {
        objectComponents[i][index] = -1;
    }
    ++objectCount;
    GameObject& object = objects[index];
    object.Parent() = &root;
    object.index = index;
    object.world = this;
    return &object;
}

void GameWorld::Update(float dt) {
    for(auto system : systems) {
        system->Update(dt);
    }
    DoActions(createActions);
    DoActions(removeActions);
}

void GameWorld::Render() {
    for(auto system : systems) {
        system->Render();
    }
}

int GameWorld::ObjectCount() const {
    return objectCount;
}

int GameWorld::CapacityCount() const {
    return (int)objects.size();
}

void GameWorld::Clear() {
    IterateObjects([](GameObject* o) {
        o->SetEnabled(false);
    });

    objects.clear();
    objectsFreeIndicies.clear();
    objectCount = 0;
    for(int i=0; i<MaxComponents; ++i) {
        if (components[i]) {
            components[i]->Clear();
        }
        objectComponents[i].clear();
    }
}

void GameWorld::Trim() {
    for(int i=0; i<MaxComponents; ++i) {
        if (components[i]) {
            components[i]->Trim();
        }
    }
    
    int smallestSize = 0;
    for(int i = (int)objects.size() - 1; i>=0; --i) {
        if (objects[i].index>=-1) {
            smallestSize = i + 1;
            break;
        }
    }
    if (smallestSize<objects.size()) {
        for(int i=0; i<MaxComponents; ++i) {
            objectComponents[i].resize(smallestSize);
        }
        objects.resize(smallestSize);
        for(int i=0; i<objectsFreeIndicies.size(); ++i) {
            if (objectsFreeIndicies[i]>=smallestSize) {
                objectsFreeIndicies.erase(objectsFreeIndicies.begin() + i);
                --i;
            }
        }
    }
}

IGameSystem* GameWorld::TryAddSystem(SystemID id, std::function<IGameSystem *(std::vector<int>& components)> constructor) {
    if (id>=systemsIndexed.size()) {
        systemsIndexed.resize(id + 1, 0);
    }
    IGameSystem* system = systemsIndexed[id];
    if (!system) {
        std::vector<int> componentIndices;
        system = constructor(componentIndices);
        for(auto c : componentIndices) {
            system->componentMask[c] = true;
            if (c>=systemsPerComponent.size()) {
                systemsPerComponent.resize(c + 1);
            }
            systemsPerComponent[c].push_back(system);
        }
        systemsIndexed[id] = system;
        systems.push_back(system);
        system->Initialize();
        
        IterateObjects([system](GameObject* o) {
            if ((o->data->enabledComponents & system->componentMask) == system->componentMask) {
                system->objects.push_back(o);
                system->ObjectAdded(o);
            }
        });
    }
    return system;
}

void GameWorld::TryRemoveSystem(SystemID id) {
    if (id>=systemsIndexed.size()) return;
    IGameSystem* system = systemsIndexed[id];
    if (!system) return;
    
    IterateObjects([system](GameObject* o) {
        if ((o->data->enabledComponents & system->componentMask) == system->componentMask) {
            system->ObjectRemoved(o);
            auto& objects = system->objects;
            objects.erase(std::find(objects.begin(), objects.end(), o));
        }
    });

    
    for(int i=0; i<MaxComponents; ++i) {
        if (system->componentMask[i]) {
            auto& list = systemsPerComponent[i];
            list.erase(std::find(list.begin(), list.end(), system));
        }
    }
    
    systems.erase(std::find(systems.begin(), systems.end(), system));
    systemsIndexed[id] = 0;
    delete system;
}

void GameWorld::DoActions(Actions &actions) {
    for(auto action : actions) {
        action();
    }
    actions.clear();
}

void GameWorld::IterateObjects(std::function<void (GameObject *)> callback) {
    for(auto& o : objects) {
        if (o.index >= 0) {
            callback(&o);
        }
    }
}