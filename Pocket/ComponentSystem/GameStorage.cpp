//
//  GameStorage.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 29/12/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "GameStorage.hpp"
#include "GameScene.hpp"

using namespace Pocket;

GameStorage::GameStorage() : componentTypesCount(0) {
    objects.count = 0;
}

void GameStorage::AddComponentType(ComponentId componentId, const ComponentTypeFunction& function) {
    if (componentId>=components.size()) {
        componentTypesCount = componentId + 1;
        components.resize(componentTypesCount);
        
        if (objects.defaultObject.activeComponents.Size()<componentTypesCount) {
            objects.defaultObject.activeComponents.Resize(componentTypesCount);
            for(auto& o : objects.entries) {
                o.activeComponents.Resize(componentTypesCount);
                o.enabledComponents.Resize(componentTypesCount);
                o.componentIndicies.resize(componentTypesCount);
            }
        }
    }
    
    if (!components[componentId].container) {
        function(components[componentId]);
    }
}

void GameStorage::AddSystemType(SystemId systemId, const SystemTypeFunction& function) {
    if (systemId>=systems.size()) {
        systems.resize(systemId + 1);
    }
    
    SystemInfo& systemInfo = systems[systemId];
    
    if (!systemInfo.createFunction) {
        std::vector<ComponentId> componentIndices;
        function(systemInfo, componentIndices);
        Bitset systemBitset;
        for(auto c : componentIndices) {
            if (c>=systemBitset.Size()) {
                systemBitset.Resize(c + 1);
            }
            systemBitset.Set(c, true);
            components[c].systemsUsingComponent.push_back(systemId);
        }
        systemInfo.bitset = systemBitset;
    }
}

void GameStorage::RemoveSystemType(SystemId systemId) {
    SystemInfo& systemInfo = systems[systemId];
    if (!systemInfo.createFunction) return;
//    scenes.Iterate([this, &systemInfo, systemId] (GameScene* scene) {
//        if (systemId>=scene->systemsIndexed.size()) return;
//        IGameSystem* system = scene->systemsIndexed[systemId];
//        if (!system) return;
//        if (system->ObjectCount()>0) {
//            scene->IterateObjects([&systemInfo, system] (GameObject* object) {
//                if ( systemInfo.bitset.Contains(object->enabledComponents)) {
//                    system->ObjectRemoved(object);
//                    system->RemoveObject(object);
//                }
//            });
//            RemoveActiveSystem(system);
//        }
//        systemInfo.deleteFunction(system);
//        scene->systemsIndexed[systemId] = 0;
//    });

    for(int i=0; i<systemInfo.bitset.Size(); ++i) {
        if (systemInfo.bitset[i]) {
            auto& list = components[i].systemsUsingComponent;
            list.erase(std::find(list.begin(), list.end(), systemId));
        }
    }
    systemInfo.createFunction = 0;
    systemInfo.deleteFunction = 0;
    systemInfo.bitset.Reset();
}

bool GameStorage::TryGetComponentIndex(const std::string& componentName, int& index) {
    for(int i=0; i<components.size(); ++i) {
        if (components[i].name == componentName) {
            index = i;
            return true;
        }
    }
    return false;
}

bool GameStorage::TryGetComponentIndex(const std::string& componentName, int& index, bool& isReference) {
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

ComponentTypeCollection GameStorage::GetComponentTypes() const {
    ComponentTypeCollection componentTypes;
    for(auto& c : components) {
        componentTypes.push_back({ c.name, c.getTypeInfo });
    }
    return componentTypes;
}

void GameStorage::SerializeAndRemoveComponents(std::ostream& stream, const SerializePredicate &predicate) {
//    minijson::writer_configuration config;
//    config = config.pretty_printing(true);
//    minijson::object_writer writer(stream, config);
//    objects.Iterate([&writer, &predicate](GameObject* object) {
//        object->WriteJsonComponents(writer, predicate);
//        object->RemoveComponents(predicate);
//    });
//    writer.close();
}

void GameStorage::DeserializeAndAddComponents(std::istream &jsonStream) {
//    minijson::istream_context context(jsonStream);
//
//    try {
//        GameObject::AddReferenceComponentList addReferenceComponents;
//
//        std::vector<GameObject*> objectsList;
//        objects.Iterate([&](GameObject* object) {
//            objectsList.push_back(object);
//        });
//
//        int index = 0;
//        minijson::parse_object(context, [&] (const char* n, minijson::value v) {
//            GameObject* object = objectsList[index];
//            index++;
//            if (v.type() == minijson::Array) {
//                minijson::parse_array(context, [&] (minijson::value v) {
//                    if (v.type() == minijson::Object) {
//                        minijson::parse_object(context, [&] (const char* n, minijson::value v) {
//                            object->AddComponent(addReferenceComponents, context, n);
//                        });
//                    }
//                });
//            }
//        });
//
//        GameObject* object;
//        int componentID;
//        GameObject* referenceObject;
//        while (GameObject::GetAddReferenceComponent(addReferenceComponents, &object, componentID, &referenceObject)) {
//            if (referenceObject) {
//                object->AddComponent(componentID, referenceObject);
//            }
//        }
//    } catch (minijson::parse_error e) {
//        std::cout << e.what() << std::endl;
//    }
//    GameObject::EndGetAddReferenceComponent();
}

