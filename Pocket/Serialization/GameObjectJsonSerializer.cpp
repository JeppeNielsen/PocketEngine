//
//  GameObjectJsonSerializer.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 29/12/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "GameObjectJsonSerializer.hpp"
#include "GameObject.hpp"
#include "GameStorage.hpp"
#include "GameScene.hpp"
#include "GameObjectHandle.hpp"

using namespace Pocket;

bool GameObjectJsonSerializer::Serialize(Pocket::GameObject *object, std::ostream &stream, const SerializePredicate &predicate) {
    minijson::writer_configuration config;
    config = config.pretty_printing(true);
    minijson::object_writer writer(stream, config);
    WriteJson(object, writer, predicate);
    writer.close();
    return true;
}

void GameObjectJsonSerializer::WriteJson(GameObject* object, minijson::object_writer& writer, const SerializePredicate& predicate) const {

    minijson::object_writer gameObject = writer.nested_object("GameObject");

    if (object->IsRoot()) {
        gameObject.write("id", object->rootId);
        gameObject.write("guid", object->scene->guid);
        gameObject.write("counter", object->scene->idCounter);
    } else {
        gameObject.write("id", object->rootId);
    }
    
    WriteJsonComponents(object, gameObject, predicate);
    
    if (!object->children.empty()) {
        minijson::array_writer children_object = gameObject.nested_array("Children");
        for(auto child : object->children) {
            if (predicate && !predicate(child, -1)) {
                continue;
            }
            minijson::object_writer child_object = children_object.nested_object();
            WriteJson(child, child_object, predicate);
            child_object.close();
        }
        children_object.close();
    }
    
    gameObject.close();
}

void GameObjectJsonSerializer::WriteJsonComponents(GameObject* object, minijson::object_writer& writer, const SerializePredicate& predicate) const {

    minijson::array_writer components = writer.nested_array("Components");
    
    GameStorage* storage = object->scene->storage;
    
    if (object->activeComponents.Size()>0) {
        for(int i=0; i<storage->components.size(); ++i) {
            if (object->activeComponents[i] && !(predicate && !predicate(object, i))) {
            
                int ownerIndex = storage->components[i].container->GetOwner(object->componentIndicies[i]);
                bool isReference = (ownerIndex != object->index) && ownerIndex>=0;
                GameObject* componentOwner;
                if (isReference) {
                    componentOwner = &storage->objects.entries[ownerIndex];
                    if (predicate && !predicate(componentOwner, i)) {
                        continue;
                    }
                } else {
                    componentOwner = 0;
                }
                SerializeComponent(object, i, components, isReference, componentOwner);
            }
        }
    }
    components.close();
}

void GameObjectJsonSerializer::SerializeComponent(GameObject* object, int componentID, minijson::array_writer& writer, bool isReference, const GameObject* referenceObject ) const {
    minijson::object_writer componentWriter = writer.nested_object();
    
    GameStorage* storage = object->scene->storage;
    
    auto& componentInfo = storage->components[componentID];
    
    if (!isReference) {
        minijson::object_writer jsonComponent = componentWriter.nested_object(componentInfo.name.c_str());
        if (componentInfo.getTypeInfo) {
            auto type = componentInfo.getTypeInfo(object);
            type.Serialize(jsonComponent);
        }
        jsonComponent.close();
    } else {
        std::string referenceName = componentInfo.name + ":ref";
        minijson::object_writer jsonComponent = componentWriter.nested_object(referenceName.c_str());
        if (!referenceObject) {
            jsonComponent.write("id", "");
        } else {
            if (referenceObject->scene == object->scene) {
                jsonComponent.write("id", referenceObject->rootId);
            } else {
                std::stringstream s;
                s<<referenceObject->rootId;
                jsonComponent.write("id", referenceObject->scene->guid +":"+s.str());
            }
        }
        jsonComponent.close();
    }
    componentWriter.close();
}


void GameObjectJsonSerializer::AddComponent(GameObject* object, AddReferenceComponentList& addReferenceComponents, minijson::istream_context& context, std::string componentName) {
    GameStorage* storage = object->scene->storage;
    int componentID;
    bool isReference;
    if (storage->TryGetComponentIndex(componentName, componentID, isReference) && !object->activeComponents[componentID]) {
        if (!isReference) {
            object->AddComponent(componentID);
            auto& componentInfo = storage->components[componentID];
            if (componentInfo.getTypeInfo) {
                auto type = componentInfo.getTypeInfo(object);
                type.Deserialize(context);
                
                for(auto& f : type.fields) {
                    if (f->GetTypeIndex() == TypeIndexList::Index<GameObjectHandle>()) {
                        FieldInfo<GameObjectHandle>* handleField = static_cast<FieldInfo<GameObjectHandle>*>(f.get());
                        handleField->field->SetWorld(object->World());
                    }
                }
                
            } else {
                minijson::ignore(context);
            }
        } else {
            std::string referenceID = "";
            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                std::string id = n;
                if (id == "id") {
                    addReferenceComponents.push_back({ object, componentID, v.as_string() });
                } else {
                    minijson::ignore(context);
                }
            });
        }
    } else {
        minijson::ignore(context);
    }
}

std::map<std::pair<std::string, GameScene*>, GameObject*> addReferenceComponentObjects;

bool GameObjectJsonSerializer::GetAddReferenceComponent(AddReferenceComponentList& addReferenceComponents, Pocket::GameObject **object, int &componentID, GameObject** referenceObject) {
    if (addReferenceComponents.empty()) return false;
    auto& refObj = addReferenceComponents.back();
    *object = refObj.object;
    componentID = refObj.componentID;
    
    GameScene* scene = refObj.object->scene;
    
    auto pair = std::make_pair(refObj.referenceId, scene);
    auto it = addReferenceComponentObjects.find(pair);
    
    if (it == addReferenceComponentObjects.end()) {
    
        size_t colonLocation = refObj.referenceId.rfind(":");
        GameScene* referenceScene;
        int objectId;
        if (colonLocation==-1) {
            referenceScene = scene;
            objectId = ::atoi(refObj.referenceId.c_str());
        } else {
            std::string sceneId = refObj.referenceId.substr(0, colonLocation);
            std::string objectIdStr = refObj.referenceId.substr(colonLocation+1, refObj.referenceId.size() - colonLocation-1);
            //referenceScene = scene->world->TryGetScene(sceneId);
            objectId = ::atoi(objectIdStr.c_str());
        }
        
        if (!referenceScene) return false;
        
        *referenceObject = referenceScene->FindObject(objectId);
        addReferenceComponentObjects.insert(std::make_pair(pair, *referenceObject));
    } else {
        *referenceObject = it->second;
    }
    addReferenceComponents.pop_back();
    return true;
}

void GameObjectJsonSerializer::EndGetAddReferenceComponent() {
    addReferenceComponentObjects.clear();
}


GameObject* GameObjectJsonSerializer::Deserialize(Pocket::GameScene *scene, std::istream &stream, const std::function<void (GameObject *)> &objectCreated) {

    minijson::istream_context context(stream);
    GameObject* object = 0;
    try {
        AddReferenceComponentList addReferenceComponents;
        object = LoadObject(addReferenceComponents, scene->root, context, objectCreated);
        
        GameObject* object;
        int componentID;
        GameObject* referenceObject;
        while (GetAddReferenceComponent(addReferenceComponents, &object, componentID, &referenceObject)) {
            if (referenceObject) {
                object->AddComponent(componentID, referenceObject);
            }
        }
    } catch (minijson::parse_error e) {
        std::cout << e.what() << std::endl;
    }
    EndGetAddReferenceComponent();
    return object;
}

GameObject* GameObjectJsonSerializer::LoadObject(AddReferenceComponentList& addReferenceComponents, GameObject* parent, minijson::istream_context &context, const std::function<void(GameObject*)>& objectCreated) {
    GameObject* object = 0;
     minijson::parse_object(context, [&] (const char* n, minijson::value v) {
        if (v.type() == minijson::Object) {
            if (parent->IsRoot()) {
                object = parent;
            } else {
                object = parent->scene->CreateEmptyObject(parent, parent->scene, false);
            }
            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                std::string name = n;
                if (name == "id" && v.type() == minijson::Number) {
                    object->rootId = (int)v.as_long();
                } else if (name == "Components" && v.type() == minijson::Array && object) {
                    minijson::parse_array(context, [&] (minijson::value v) {
                        if (v.type() == minijson::Object) {
                            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                                AddComponent(object, addReferenceComponents, context, n);
                            });
                        }
                    });
                } else if (name == "Children" && v.type() == minijson::Array && object) {
                    minijson::parse_array(context, [&] (minijson::value v) {
                        LoadObject(addReferenceComponents, object, context, objectCreated);
                    });
                } else if (!parent && name == "guid" && v.type() == minijson::String) {
                    object->scene->guid = std::string(v.as_string());
                } else if (!parent && name == "counter" && v.type() == minijson::Number) {
                    object->scene->idCounter = (int)v.as_long();
                }
                
                if (objectCreated) {
                    objectCreated(object);
                }
            });
        }
    });
    return object;
}





