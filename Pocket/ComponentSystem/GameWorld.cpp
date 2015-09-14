//
//  World.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 9/24/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "GameWorld.hpp"
#ifdef ENABLE_SCRIPTING
#include "ScriptWorld.hpp"
#include "ScriptSystem.hpp"
#endif

#include "minijson_writer.hpp"
#include <iostream>

using namespace Pocket;

GameWorld::GameWorld() {
    for (int i=0; i<GameComponentTypeFactory::componentIdCounter; i++) {
        componentTypes.push_back(GameComponentTypeFactory::createdComponentTypes->at(i)->Clone());
    }
    scriptWorld = 0;
}

GameWorld::~GameWorld() {
    
    DeleteAllObjects();
    for (size_t i=0; i<componentTypes.size(); i++) {
        delete componentTypes[i];
    }
    for (size_t i = 0; i<activeObjects.size(); i++) {
        delete activeObjects[i];
    }
    for (size_t i=0; i<freeObjects.size(); i++) {
        delete freeObjects[i];
    }
    
#ifdef ENABLE_SCRIPTING
    if (scriptWorld) {
        scriptWorld->Destroy();
        delete scriptWorld;
        scriptWorld = 0;
    }
#endif

    for (int i=0; i<systems.size(); i++) {
        delete systems[i];
    }
}

const ObjectCollection& GameWorld::Objects() { return activeObjects; }
const ObjectCollection& GameWorld::Children() { return children; }

void GameWorld::DeleteAllObjects() {
    children.clear();
    while (!activeObjects.empty()) {
        UpdateRemovedObject(activeObjects.back());
    }
}

GameObject* GameWorld::CreateObject() {
    if (freeObjects.empty()) {
        for (size_t i=0; i<16; i++) {
            GameObject* object = new GameObject();
            object->Initialize(componentTypes.size());
            object->world = this;
            object->systemIndicies = new GameObject::SystemIndicies[systems.size()];
            freeObjects.push_back(object);
        }
    }
    GameObject* object = freeObjects.back();
    object->indexInList = (unsigned)activeObjects.size();
    freeObjects.pop_back();
    activeObjects.push_back(object);
    children.push_back(object);
    return object;
}

void GameWorld::UpdateRemovedObjects() {
    while (!removedObjects.empty()) {
        RemovedObjects temp = removedObjects;
        removedObjects.clear();
        for (RemovedObjects::iterator it = temp.begin(); it!=temp.end(); ++it) {
            UpdateRemovedObject(*it);
        }
    }
}

void GameWorld::UpdateRemovedObject(GameObject *object) {

    for (size_t i=0; i<componentTypes.size(); i++) {
        if (object->components[i]) {
            componentTypes[i]->UpdateRemovedObject(object);
        }
    }
    
    if (!object->Parent() && !children.empty()) {
        GameObject* lastChild = children.back();
        children[object->childIndex] = lastChild;
        lastChild->childIndex = object->childIndex;
        children.pop_back();
    }
    object->Parent = 0;
    freeObjects.push_back(object);
    GameObject* lastObject = activeObjects.back();
    lastObject->indexInList = object->indexInList;
    activeObjects[object->indexInList] = lastObject;
    activeObjects.pop_back();
    for (int i=0; i<objectIDs.size(); ++i) {
        if (objectIDs[i].object == object) {
            objectIDs.erase(objectIDs.begin() + i);
            break;
        }
    }
    object->ClearPointers();
}

void GameWorld::UpdateChangedComponents() {
    while (!changedComponentTypes.empty()) {
        ComponentTypes temp = changedComponentTypes;
        changedComponentTypes.clear();
        for (size_t i = 0; i<temp.size(); i++) {
            temp[i]->UpdateChangedObjects();
        }
    }
}

void GameWorld::UpdateRemovedComponents() {
    while (!removedComponentTypes.empty()) {
        ComponentTypes temp = removedComponentTypes;
        removedComponentTypes.clear();
        for (size_t i=0; i<temp.size(); i++) {
            temp[i]->UpdateRemovedObjects();
        }
    }
}

void GameWorld::Update(float dt) {
    UpdateChangedComponents();
    UpdateRemovedComponents();
    UpdateRemovedObjects();
    for (size_t i = 0; i<systems.size(); i++) {
        systems[i]->Update(dt);
    }
    //UpdateRemovedComponents();
    //UpdateRemovedObjects();
}

void GameWorld::Render() {
    for (size_t i = 0; i<systems.size(); i++) {
        systems[i]->Render();
    }
}

void* GameWorld::AddComponent(GameObject* object, int componentID) {
    if (object->components[componentID]) return object->components[componentID];
    IGameComponentType* type = componentTypes[componentID];
    void* component = type->AddComponent();
    object->components[componentID] = component;
    object->ownedComponents |= type->mask;
    EnableComponent(object, componentID, true);
    return component;
}

void* GameWorld::AddComponent(GameObject *object, int componentID, GameObject *referenceObject) {
    if (object->components[componentID]) return object->components[componentID];
    void* referenceComponent = referenceObject->components[componentID];
    if (!referenceComponent) return 0; // no such component on referenced gameobject
    IGameComponentType* type = referenceObject->world->componentTypes[componentID];
    type->AddReference(referenceComponent);
    object->components[componentID] = referenceComponent;
    EnableComponent(object, componentID, true);
    return referenceComponent;
}

void GameWorld::RemoveComponent(GameObject *object, int componentID) {
    IGameComponentType* type = componentTypes[componentID];
    if (!type->isInRemovedList) {
        type->isInRemovedList = true;
        removedComponentTypes.push_back(type);
    }
    type->removedObjects.insert(object);
}

void GameWorld::EnableComponent(GameObject *object, int componentID, bool enable) {
    IGameComponentType* type = componentTypes[componentID];
    bool isEnabled = (object->enabledComponents & type->mask) == type->mask;
    if (isEnabled == enable) return;
    if (enable) {
        object->enabledComponents |= type->mask;
    } else {
        object->enabledComponents &= type->maskInverse;
    }
    if (!type->isInEnabledChangedList) {
        type->isInEnabledChangedList = true;
        changedComponentTypes.push_back(type);
    }
    type->changedObjects.push_back(object);
}

void* GameWorld::CloneComponent(GameObject *object, int componentID, GameObject* source) {
    if (object->components[componentID]) return object->components[componentID];
    void* sourceComponent = source->components[componentID];
    if (!sourceComponent) return 0; // source object does not have this component type
    IGameComponentType* type = componentTypes[componentID];
    void* component;
    if ((source->ownedComponents & type->mask) == type->mask) {
        component = type->CloneComponent(sourceComponent);
        object->ownedComponents |= type->mask;
    } else {
        type->AddReference(sourceComponent);
        component = sourceComponent;
    }
    object->components[componentID] = component;
    EnableComponent(object, componentID, (source->enabledComponents & type->mask) == type->mask);
    return component;
}

void GameWorld::UpdatePointers(Pocket::GameObject *object, int componentID) {
    void* component = object->components[componentID];
    IGameComponentType* type = componentTypes[componentID];
    if ((object->ownedComponents & type->mask) == type->mask) {
        type->UpdatePointers(component, pointerMap, pointerCounter);
    }
}

void GameWorld::AddSystem(GameSystem* system, int componentID) {
    IGameComponentType* type = componentTypes[componentID];
    system->aspect |= type->mask;
    type->systems.push_back(system);
}

const GameWorld::ComponentTypes& GameWorld::ComponentTypesList() { return componentTypes; }

void GameWorld::WriteJsonComponent(minijson::array_writer& writer, GameObject* object, int componentID) {
    IGameComponentType* type = componentTypes[componentID];
    std::string* referenceID;
    bool isReference = object->IsComponentReference(componentID);
    if (isReference) {
        referenceID = FindIDFromReferenceObject(object, componentID);
    } else {
        referenceID = 0;
    }
    type->WriteJson(writer, object->components[componentID], isReference, referenceID);
}

void GameWorld::ReadJsonComponent(minijson::istream_context &context, GameObject *object, int componentID) {
    IGameComponentType* type = componentTypes[componentID];
    type->ReadJson(context, object->components[componentID]);
}

GameObject* GameWorld::CreateObjectFromJson(std::istream &jsonStream) {
    minijson::istream_context context(jsonStream);
    GameObject* object = CreateGameObjectJson(context);
    return object;
}

GameObject* GameWorld::CreateGameObjectJson(minijson::istream_context &context) {
    GameObject* object = 0;
    
    try {
         minijson::parse_object(context, [&] (const char* n, minijson::value v) {
            std::string name = n;
            if (name == "GameObject" && v.type() == minijson::Object) {
                object = CreateObject();
                minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                    std::string name = n;
                    if (name == "Components" && v.type() == minijson::Array && object) {
                        minijson::parse_array(context, [&] (minijson::value v) {
                            if (v.type() == minijson::Object) {
                                minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                                    std::string componentName = n;
                                    int componentID = GameComponentTypeFactory::ComponentIDFromName(componentName);
                                    if (componentID!=-1) {
                                        if (!object->components[componentID]) { // only allow one type of component for each object
                                            AddComponent(object, componentID);
                                            ReadJsonComponent(context, object, componentID);
                                        } else {
                                            std::cout<<"Only one component per type allowed per object"<<std::endl;
                                            minijson::ignore(context);
                                        }
                                    } else if(componentName.size()>4) {
                                        size_t refLocation = componentName.rfind(":ref");
                                        if (refLocation!=-1) {
                                            componentName = componentName.substr(0, refLocation);
                                            componentID = GameComponentTypeFactory::ComponentIDFromName(componentName);
                                            if (componentID!=-1) {
                                                std::string referenceID = "";
                                                minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                                                    std::string id = n;
                                                    if (id == "id" && v.type()==minijson::String) {
                                                        referenceID = v.as_string();
                                                        GameObject* referenceObject = FindObjectFromID(referenceID);
                                                        if (!referenceObject) {
                                                            //object not found with id, try assign first object with this component
                                                            referenceObject = FindFirstObjectWithComponentID(componentID);
                                                        }
                                                        if (referenceObject) {
                                                            AddComponent(object, componentID, referenceObject);
                                                        }
                                                    } else {
                                                        minijson::ignore(context);
                                                    }
                                                });
                                                std::cout << "component reference found for component : " <<componentName<<std::endl;
                                            }
                                            minijson::ignore(context);
                                        }
                                    }
                                });
                            }
                        });
                    } else if (name == "Children" && v.type() == minijson::Array && object) {
                        minijson::parse_array(context, [&] (minijson::value v) {
                            GameObject* child = CreateGameObjectJson(context);
                            if (child) {
                                child->Parent = object;
                            }
                        });
                    }
                });
            }
         });
    } catch (std::exception e) {
  
    }
    return object;
}

void GameWorld::CreateObjectID(GameObject *object, const std::string &id) {
    for (auto& objectID : objectIDs) {
        if (objectID.object == object)  {
            objectID.id = id;
            return;
        }
    }
    objectIDs.push_back({object, id});
}

GameObject* GameWorld::FindObjectFromID(const std::string &id) {
    for (auto& objectID : objectIDs) {
        if (objectID.id == id) return objectID.object;
    }
    return 0;
}

std::string* GameWorld::FindIDFromObject(GameObject *object) {
    for (auto& objectID : objectIDs) {
        if (objectID.object == object) return &objectID.id;
    }
    return 0;
}

std::string* GameWorld::FindIDFromReferenceObject(GameObject* referenceObject, int componentID) {
    for (auto& objectID : objectIDs) {
        if (objectID.object->components[componentID] &&
            objectID.object->components[componentID] == referenceObject->components[componentID] &&
            !objectID.object->IsComponentReference(componentID)) {
            return &objectID.id;
        }
    }
    return 0;
}

GameObject* GameWorld::FindFirstObjectWithComponentID(int componentID) {
    for(GameObject* object : activeObjects) {
        if (object->components[componentID]) return object;
    }
    return 0;
}

#ifdef ENABLE_SCRIPTING

void GameWorld::InitializeScripts() {
    scriptWorld = new ScriptWorld();
    scriptWorld->Initialize(this);
}

bool GameWorld::Build(std::vector<std::string> scriptFilenames) {
    return scriptWorld->Build(scriptFilenames);
}

bool GameWorld::CallMain() {
    return scriptWorld->CallMain();
}

ScriptWorld* GameWorld::GetScriptWorld() { return scriptWorld; }

#endif
