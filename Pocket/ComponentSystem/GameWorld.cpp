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

std::map<std::string, GameObject*> loadedObjects;

GameWorld::GameWorld() {
    root.world = this;
    objectCount = 0;
    numComponentTypes = 0;
}

GameWorld::~GameWorld() {
    Clear();

    for(auto& system : systemsIndexed) {
        if (system.system) {
            if (system.deleteFunction) {
                system.deleteFunction();
            } else {
                delete system.system;
            }
        }
    }
    for(int i=0; i<numComponentTypes; ++i) {
        delete components[i];
    }
}

const GameObject* GameWorld::Root() { return &root; }

GameObject* GameWorld::CreateObject() {
    int index;
    if (objectsFreeIndicies.empty()) {
        index = (int)objects.size();
        objects.resize(index + 1);
        if (numComponentTypes>0 && index>=objectComponents[0].size()) {
            for(int i=0; i<numComponentTypes; i++) {
                objectComponents[i].resize(index + 32);
            }
        }
        objects[index].object.data->activeComponents.Resize(numComponentTypes);
        objects[index].object.data->enabledComponents.Resize(numComponentTypes);
    } else {
        index = objectsFreeIndicies.back();
        objectsFreeIndicies.pop_back();
    }
    
    for(int i=0; i<numComponentTypes; i++) {
        objectComponents[i][index].index = -1;
    }
    ++objectCount;
    GameObject& object = objects[index].object;
    object.data->activeComponents.Reset();
    object.data->enabledComponents.Reset();
    object.data->removed = false;
    root.data->children.push_back(&object);
    object.index = index;
    object.world = this;
    return &object;
}

GameObject* GameWorld::CreateObject(std::istream &jsonStream, std::function<void(GameObject*)> onCreated) {
    minijson::istream_context context(jsonStream);
    GameObject* object = 0;
    try {
        object = LoadObject(context, onCreated);
        
        GameObject* object;
        int componentID;
        std::string referenceID;
        while (GameObject::GetAddReferenceComponent(&object, componentID, referenceID)) {
            GameObject* referenceObject = 0;
            auto foundObjectWithID = loadedObjects.find(referenceID);
            if (foundObjectWithID!=loadedObjects.end()) {
                referenceObject = foundObjectWithID->second;
            } else {
                referenceObject = FindObjectFromID(referenceID);
                if (!referenceObject) {
                    //object not found with id, try assign first object with this component
                    referenceObject = FindFirstObjectWithComponentID(componentID);
                }
            }
            object->AddComponent(componentID, referenceObject);
        }
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
    return object;
}

void GameWorld::Update(float dt) {
    DoActions(delayedActions);
    for(auto system : systems) {
        system->Update(dt);
    }
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
    for(int i=0; i<numComponentTypes; ++i) {
        if (components[i]) {
            components[i]->Clear();
        }
        objectComponents[i].clear();
    }
    components.clear();
    objectComponents.clear();
    numComponentTypes = 0;
}

void GameWorld::Trim() {

    DoActions(delayedActions);
    
    for(int i=0; i<numComponentTypes; ++i) {
        if (components[i]) {
            components[i]->Trim();
        }
    }
    
    int smallestSize = 0;
    for(int i = (int)objects.size() - 1; i>=0; --i) {
        if (!objects[i].object.IsRemoved()) {
            smallestSize = i + 1;
            break;
        }
    }
    if (smallestSize<objects.size()) {
        for(int i=0; i<numComponentTypes; ++i) {
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

const GameWorld::ComponentInfos& GameWorld::ComponentTypes() {
    return componentInfos;
}

IGameSystem* GameWorld::TryAddSystem(SystemID id, std::function<IGameSystem *(std::vector<int>& components)> constructor) {
    if (id>=systemsIndexed.size()) {
        systemsIndexed.resize(id + 1);
    }
    IGameSystem* system = systemsIndexed[id].system;
    if (!system) {
        SystemEntry& systemEntry = systemsIndexed[id];
        std::vector<int> componentIndices;
        system = systemEntry.system = constructor(componentIndices);
        Bitset systemBitset;
        for(auto c : componentIndices) {
            if (c>=systemBitset.Size()) {
                systemBitset.Resize(c + 1);
            }
            systemBitset.Set(c, true);
            if (c>=systemsPerComponent.size()) {
                systemsPerComponent.resize(c + 1);
            }
            systemsPerComponent[c].push_back(id);
        }
        systems.push_back(system);
        systemEntry.bitset = systemBitset;
        systemEntry.deleteFunction = 0;
        systemEntry.system->Initialize();
        
        IterateObjects([system, &systemBitset](GameObject* o) {
            if (systemBitset.Contains(o->data->enabledComponents)) {
                system->AddObject(o);
                system->ObjectAdded(o);
            }
        });
    }
    return system;
}

void GameWorld::TryRemoveSystem(SystemID id) {
    if (id>=systemsIndexed.size()) return;
    SystemEntry& systemEntry = systemsIndexed[id];
    if (!systemEntry.system) return;
    IGameSystem* system = systemEntry.system;
    Bitset& systemBitset = systemEntry.bitset;
    
    IterateObjects([system, &systemBitset](GameObject* o) {
        if (systemBitset.Contains(o->data->enabledComponents)) {
            system->ObjectRemoved(o);
            system->RemoveObject(o);
        }
    });
    
    for(int i=0; i<numComponentTypes; ++i) {
        if (systemBitset[i]) {
            auto& list = systemsPerComponent[i];
            list.erase(std::find(list.begin(), list.end(), id));
        }
    }
    
    systems.erase(std::find(systems.begin(), systems.end(), system));
    
    systemEntry.system = 0;
    systemEntry.bitset.Reset();
    
    if (systemEntry.deleteFunction) {
        systemEntry.deleteFunction();
    } else {
        delete system;
    }
    systemEntry.deleteFunction = 0;
}

void GameWorld::DoActions(Actions &actions) {
    for(int i=0; i<actions.size();++i) {
        actions[i]();
    }
    actions.clear();
}

void GameWorld::IterateObjects(std::function<void (GameObject *)> callback) {
    for(auto& o : objects) {
        if (!o.object.IsRemoved()) {
            callback(&o.object);
        }
    }
}

void GameWorld::TryAddComponentContainer(ComponentID id, std::function<IContainer *(GameObject::ComponentInfo&)> &&constructor) {
    if (id>=components.size()) {
        int count = id + 1;
        components.resize(count, 0);
        componentInfos.resize(count);
        
        IterateObjects([count](GameObject* o) {
            o->data->activeComponents.Resize(count);
            o->data->enabledComponents.Resize(count);
        });
        numComponentTypes = count;
        objectComponents.resize(count);
        for(int i=0; i<count; ++i) {
            objectComponents[i].resize(objects.size(), {-1,0});
        }
    }

    if (!components[id]) {
        components[id] = constructor(componentInfos[id]);
    }
}

void GameWorld::AddObjectID(Pocket::GameObject *object, std::string id) {
    for(auto& o : objectIDs) {
        if (o.object == object) {
            o.id = id;
            return;
        }
    }
    objectIDs.push_back({object, id});
}

std::string* GameWorld::GetObjectID(Pocket::GameObject *object) {
    for(auto& o : objectIDs) {
        if (o.object == object) {
            return &o.id;
        }
    }
    return 0;
}

std::string* GameWorld::FindIDFromReferenceObject(GameObject* referenceObject, int componentID) {
    for (auto& objectID : objectIDs) {
        if (!objectID.object->data->activeComponents[componentID]) continue;
        if (objectID.object->GetComponent(componentID) != referenceObject->GetComponent(componentID)) continue;
        return &objectID.id;
    }
    return 0;
}

GameObject* GameWorld::FindObjectFromID(const std::string &id) {
    for (auto& objectID : objectIDs) {
        if (objectID.id == id) return objectID.object;
    }
    return 0;
}

GameObject* GameWorld::FindFirstObjectWithComponentID(int componentID) {
    for(auto& entry : objects) {
        if (entry.object.IsRemoved()) continue;
        if (!entry.object.data->activeComponents[componentID]) continue;
        return &entry.object;
    }
    return 0;
}

GameObject* GameWorld::LoadObject(minijson::istream_context &context, std::function<void(GameObject*)>& onCreated) {
    GameObject* object = 0;
     minijson::parse_object(context, [&] (const char* n, minijson::value v) {
        std::string name = n;
        if (name == "GameObject" && v.type() == minijson::Object) {
            object = (GameObject*)CreateObject();
            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                std::string name = n;
                if (name == "id" && v.type() == minijson::String) {
                    loadedObjects[std::string(v.as_string())] = object;
                } else if (name == "Components" && v.type() == minijson::Array && object) {
                    minijson::parse_array(context, [&] (minijson::value v) {
                        if (v.type() == minijson::Object) {
                            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                                object->AddComponent(context, n);
                            });
                        }
                    });
                } else if (name == "Children" && v.type() == minijson::Array && object) {
                    minijson::parse_array(context, [&] (minijson::value v) {
                        GameObject* child = LoadObject(context, onCreated);
                        if (child) {
                            child->Parent() = object;
                        }
                    });
                }
                
                if (onCreated) {
                    onCreated(object);
                }
            });
        }
    });
    return object;
}

bool GameWorld::TryGetComponentIndex(const std::string& componentName, int& index) {
    for(int i=0; i<componentInfos.size(); ++i) {
        if (componentInfos[i].name == componentName) {
            index = i;
            return true;
        }
    }
    return false;
}

bool GameWorld::TryGetComponentIndex(const std::string& componentName, int& index, bool& isReference) {
    if (TryGetComponentIndex(componentName, index)) {
        isReference = false;
        return true;
    }
    if (componentName.size()>4) {
        size_t refLocation = componentName.rfind(":ref");
        if (refLocation!=-1) {
            std::string componentNameNoRef = componentName.substr(0, refLocation);
            if (TryGetComponentIndex(componentNameNoRef, index)) {
                isReference = true;
                return true;
            }
        }
    }
    return false;
}
