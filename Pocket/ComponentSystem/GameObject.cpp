//
//  GameObject.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 17/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameObject.hpp"
#include "GameWorld.hpp"

using namespace Pocket;

static bool forceSetNextParent = false;

GameObject::GameObject() { }

GameObject::~GameObject() { }

GameObject::GameObject(const GameObject& other) {
    activeComponents.Resize(other.activeComponents.Size());
    enabledComponents.Resize(other.activeComponents.Size());
    componentIndicies.resize(other.activeComponents.Size());
    
    Parent.Changed.Bind([this]() {
        assert(!IsRoot()); // roots are not allowed to have a parent
        assert(Parent!=this); // parent cannot be self
        if (!forceSetNextParent) {
            assert(Parent()->scene == this->scene); // parent needs to be within the same scene/root
            assert(Parent());// objects must have a parent
        }
        GameObject* prevParent = Parent.PreviousValue();
        GameObject* currentParent = Parent;
        
        if (removed && !forceSetNextParent) return;
        
        if (prevParent) {
            auto& children = prevParent->children;
            children.erase(std::find(children.begin(), children.end(), this));
            prevParent->WorldEnabled.HasBecomeDirty.Unbind(this, &GameObject::SetWorldEnableDirty);
        }
        
        if (currentParent) {
            auto& children = currentParent->children;
            children.push_back(this);
            currentParent->WorldEnabled.HasBecomeDirty.Bind(this, &GameObject::SetWorldEnableDirty);
            
            bool prevWorldEnabled = WorldEnabled;
            WorldEnabled.MakeDirty();
            if (WorldEnabled()!=prevWorldEnabled) {
                SetWorldEnableDirty();
            }
        }
    });
    
    WorldEnabled.Method = [this](bool& value) {
        value = (Parent) ? Parent()->WorldEnabled && Enabled : Enabled;
    };
    
    Enabled.Changed.Bind(this, &GameObject::SetWorldEnableDirty);
    
}

void GameObject::Reset() {
    removed = false;
    Parent = 0;
    Enabled = true;
    children.clear();
    Order = 0;
}

bool GameObject::HasComponent(ComponentId id) const {
    assert(id<activeComponents.Size());
    return activeComponents[id];
}

void* GameObject::GetComponent(ComponentId id) const {
    assert(id<activeComponents.Size());
    if (!activeComponents[id]) return 0;
    return scene->world->components[id].container->Get(componentIndicies[id]);
}

void GameObject::AddComponent(ComponentId id) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (activeComponents[id]) return;

    componentIndicies[id] = scene->world->components[id].container->Create(index);
    activeComponents.Set(id, true);
    scene->delayedActions.emplace_back([this, id]() {
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::AddComponent(ComponentId id, GameObject* referenceObject) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (activeComponents[id]) return;
    if (!referenceObject->activeComponents[id]) return;
    
    componentIndicies[id] = referenceObject->componentIndicies[id];
    scene->world->components[id].container->Reference(referenceObject->componentIndicies[id]);
    activeComponents.Set(id, true);
    scene->delayedActions.emplace_back([this, id]() {
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::RemoveComponent(ComponentId id) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (!activeComponents[id]) return;
    scene->delayedActions.emplace_back([this, id]() {
        if (!activeComponents[id]) {
           return; // might have been removed by earlier remove action, eg if two consecutive RemoveComponent<> was called
        }
        TrySetComponentEnabled(id, false);
        scene->world->components[id].container->Delete(componentIndicies[id], index);
        activeComponents.Set(id, false);
    });
}

void GameObject::CloneComponent(ComponentId id, GameObject* object) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (activeComponents[id]) return;
    if (!object->activeComponents[id]) return;
    componentIndicies[id] = scene->world->components[id].container->Clone(object->componentIndicies[id], index);
    activeComponents.Set(id, true);
    scene->delayedActions.emplace_back([this, id]() {
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::SetWorldEnableDirty() {
    WorldEnabled.MakeDirty();
    scene->delayedActions.emplace_back([this](){
        SetEnabled(WorldEnabled);
    });
}

void GameObject::SetEnabled(bool enabled) {
    for(int i=0; i<activeComponents.Size(); ++i) {
        if (activeComponents[i]) {
            TrySetComponentEnabled(i, enabled);
        }
    }
}

void GameObject::TrySetComponentEnabled(ComponentId id, bool enable) {
    
    enable = enable && WorldEnabled();

    bool isEnabled = enabledComponents[id];
    if (isEnabled==enable) {
        return; //cannot double enable/disable components
    }
    
    GameWorld* world = scene->world;
    GameWorld::ComponentInfo& componentInfo = world->components[id];
    
    if (enable) {
        enabledComponents.Set(id, true);
        for(auto systemIndex : componentInfo.systemsUsingComponent) {
            if (systemIndex>=scene->systemsIndexed.size()) break; // systemindex is beyond scene's systems list, thus nothing else to do
            auto& systemBitset = world->systems[systemIndex].bitset;
            bool isInterest = systemBitset.Contains(enabledComponents);
            if (isInterest) {
                IGameSystem* system = scene->systemsIndexed[systemIndex];
                if (!system) continue; // system is not part of scene/root
                system->AddObject(this);
                system->ObjectAdded(this);
                if (system->ObjectCount() == 1) {
                    scene->activeSystems.push_back(system);
                }
            }
        }
    } else {
        for(auto systemIndex : componentInfo.systemsUsingComponent) {
            if (systemIndex>=scene->systemsIndexed.size()) break; // systemindex is beyond scene's systems list, thus nothing else to do
            auto& systemBitset = world->systems[systemIndex].bitset;
            bool wasInterest = systemBitset.Contains(enabledComponents);
            if (wasInterest) {
                IGameSystem* system = scene->systemsIndexed[systemIndex];
                if (!system) continue; // system is not part of scene/root

                system->ObjectRemoved(this);
                system->RemoveObject(this);
                if (system->ObjectCount() == 0) {
                    scene->activeSystems.erase(std::find(scene->activeSystems.begin(), scene->activeSystems.end(), system));
                }
            }
        }
        enabledComponents.Set(id, false);
    }
}

void GameObject::Remove() {
    if (removed) return;
    int localIndex = index;
    scene->delayedActions.emplace_back([this, localIndex]() {
        forceSetNextParent = true;
        if (IsRoot()) {
            scene->world->RemoveRoot(this);
        } else {
            Parent = 0;
        }
        forceSetNextParent = false;
        SetEnabled(false);
        for(int i=0; i<activeComponents.Size(); ++i) {
            if (activeComponents[i]) {
                scene->world->components[i].container->Delete(componentIndicies[i], index);
                activeComponents.Set(i, false);
            }
        }
        scene->world->objects.Delete(index, 0);
    });
    removed = true;
    for(auto child : children) {
        child->Remove();
    }
}

bool GameObject::IsRemoved() const {
    return removed;
}

Handle<GameObject> GameObject::GetHandle() {
    return scene->world->objects.GetHandle(index);
}

int GameObject::RootId() const {
    return rootId;
}

GameObject* GameObject::CreateChild() {
    return scene->world->CreateEmptyObject(this, scene, true);
}

GameObject* GameObject::CreateObject() {
    return scene->root->CreateChild();
}

GameObject* GameObject::CreateChildFromJson(std::istream &jsonStream, const std::function<void (GameObject *)>& objectCreated) {
    return scene->world->CreateObjectFromJson(this, jsonStream, objectCreated);
}

GameObject* GameObject::Root() {
    return scene->root;
}

bool GameObject::IsRoot() const {
    return scene->root == this;
}

const ObjectCollection& GameObject::Children() {
    return children;
}

InputManager& GameObject::Input() { return scene->world->Input(); }

std::vector<TypeInfo> GameObject::GetComponentTypes(const std::function<bool(int componentID)>& predicate) {
    std::vector<TypeInfo> infos;
    GameWorld* world = scene->world;
    for (int i=0; i<world->components.size(); ++i) {
        if (!world->components[i].getTypeInfo) continue; // component has no type
        if (!activeComponents[i]) continue; // gameobject hasn't got component
        if (predicate && !predicate(i)) continue; // component type not allowed
        infos.emplace_back(world->components[i].getTypeInfo(this));
        infos[infos.size()-1].name = world->components[i].name;
    }
    return infos;
}

std::vector<int> GameObject::GetComponentIndicies() {
    std::vector<int> indicies;
    for (int i=0; i<activeComponents.Size(); ++i) {
        if (activeComponents[i]) {
            indicies.push_back(i);
        }
    }
    return indicies;
}





//SERIALIZATION

void GameObject::ToJson(std::ostream& stream, SerializePredicate predicate) const {
    minijson::writer_configuration config;
    config = config.pretty_printing(true);
    minijson::object_writer writer(stream, config);
    WriteJson(writer, predicate);
    writer.close();
}

void GameObject::WriteJson(minijson::object_writer& writer, SerializePredicate predicate) const {

    minijson::object_writer gameObject = writer.nested_object("GameObject");

    if (IsRoot()) {
        gameObject.write("id", rootId);
        gameObject.write("guid", scene->guid);
        gameObject.write("counter", scene->idCounter);
    } else {
        gameObject.write("id", rootId);
    }
    
    minijson::array_writer components = gameObject.nested_array("Components");
    
    GameWorld* world = scene->world;
    
    if (activeComponents.Size()>0) {
        for(int i=0; i<world->components.size(); ++i) {
            if (activeComponents[i] && !(predicate && !predicate(this, i))) {
            
                int ownerIndex = world->components[i].container->GetOwner(componentIndicies[i]);
                bool isReference = (ownerIndex != index) && ownerIndex>=0;
                GameObject* componentOwner;
                if (isReference) {
                    componentOwner = &world->objects.entries[ownerIndex];
                    if (predicate && !predicate(componentOwner, i)) {
                        continue;
                    }
                } else {
                    componentOwner = 0;
                }
                SerializeComponent(i, components, isReference, componentOwner);
            }
        }
    }
    components.close();
    
    if (!children.empty()) {
        minijson::array_writer children_object = gameObject.nested_array("Children");
        for(auto child : children) {
            if (predicate && !predicate(child, -1)) {
                continue;
            }
            minijson::object_writer child_object = children_object.nested_object();
            child->WriteJson(child_object, predicate);
            child_object.close();
        }
        children_object.close();
    }
    
    gameObject.close();
}

void GameObject::SerializeComponent(int componentID, minijson::array_writer& writer, bool isReference, const GameObject* referenceObject ) const {
    minijson::object_writer componentWriter = writer.nested_object();
    
    GameWorld* world = scene->world;
    
    GameWorld::ComponentInfo& componentInfo = world->components[componentID];
    
    if (!isReference) {
        minijson::object_writer jsonComponent = componentWriter.nested_object(componentInfo.name.c_str());
        if (componentInfo.getTypeInfo) {
            auto type = componentInfo.getTypeInfo(this);
            type.Serialize(jsonComponent);
        }
        jsonComponent.close();
    } else {
        std::string referenceName = componentInfo.name + ":ref";
        minijson::object_writer jsonComponent = componentWriter.nested_object(referenceName.c_str());
        if (!referenceObject) {
            jsonComponent.write("id", "");
        } else {
            if (referenceObject->scene == scene) {
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

struct AddReferenceComponent {
    GameObject* object;
    int componentID;
    std::string referenceId;
};

std::vector<AddReferenceComponent> addReferenceComponents;

void GameObject::AddComponent(minijson::istream_context& context, std::string componentName) {
    GameWorld* world = scene->world;
    int componentID;
    bool isReference;
    if (world->TryGetComponentIndex(componentName, componentID, isReference) && !activeComponents[componentID]) {
        if (!isReference) {
            AddComponent(componentID);
            GameWorld::ComponentInfo& componentInfo = world->components[componentID];
            if (componentInfo.getTypeInfo) {
                auto type = componentInfo.getTypeInfo(this);
                type.Deserialize(context);
            } else {
                minijson::ignore(context);
            }
        } else {
            std::string referenceID = "";
            minijson::parse_object(context, [&] (const char* n, minijson::value v) {
                std::string id = n;
                if (id == "id") {
                    addReferenceComponents.push_back({ this, componentID, v.as_string() });
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

bool GameObject::GetAddReferenceComponent(Pocket::GameObject **object, int &componentID, GameObject** referenceObject) {
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
            referenceScene = scene->world->TryGetScene(sceneId);
            objectId = ::atoi(objectIdStr.c_str());
        }
        
        *referenceObject = referenceScene->FindObject(objectId);
        addReferenceComponentObjects.insert(std::make_pair(pair, *referenceObject));
    } else {
        *referenceObject = it->second;
    }
    addReferenceComponents.pop_back();
    return true;
}

void GameObject::EndGetAddReferenceComponent() {
    addReferenceComponentObjects.clear();
}

//END SERIALIZATION



