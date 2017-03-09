//
//  GameObject.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 17/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameObject.hpp"
#include "GameWorld.hpp"
#include "GameObjectHandle.hpp"

using namespace Pocket;

static bool forceSetNextParent = false;

GameObject::GameObject() { Parent = 0; }

GameObject::~GameObject() { }

GameObject::GameObject(const GameObject& other) {
    activeComponents.Resize(other.activeComponents.Size());
    enabledComponents.Resize(other.activeComponents.Size());
    componentIndicies.resize(other.activeComponents.Size());
    
    Parent = 0;
    
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
}

void GameObject::Reset() {
    Enabled.Changed.Clear();
    removed = false;
        forceSetNextParent = true;
    Parent = 0;
    forceSetNextParent = false;
    Enabled = true;
    children.clear();
    Order = 0;
    Enabled.Changed.Bind(this, &GameObject::SetWorldEnableDirty);
}

bool GameObject::HasComponent(ComponentId id) const {
    if (id>=activeComponents.Size()) {
        return false;
    }
    return activeComponents[id];
}

void* GameObject::GetComponent(ComponentId id) const {
    if (id>=activeComponents.Size()) {
        return 0;
    }
    if (!activeComponents[id]) return 0;
    return scene->world->components[id].container->Get(componentIndicies[id]);
}

void GameObject::AddComponent(ComponentId id) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (activeComponents[id]) return;

    componentIndicies[id] = scene->world->components[id].container->Create(index);
    activeComponents.Set(id, true);
    scene->world->delayedActions.emplace_back([this, id]() {
        if (scene->ComponentCreated) {
            scene->ComponentCreated(this, id);
        }
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::AddComponent(ComponentId id, GameObject* referenceObject) {
    assert(id<activeComponents.Size());
    assert(scene->world == referenceObject->scene->world);
    if (removed) return;
    if (activeComponents[id]) return;
    if (!referenceObject->activeComponents[id]) return;
    
    componentIndicies[id] = referenceObject->componentIndicies[id];
    scene->world->components[id].container->Reference(referenceObject->componentIndicies[id]);
    activeComponents.Set(id, true);
    scene->world->delayedActions.emplace_back([this, id]() {
        if (scene->ComponentCreated) {
            scene->ComponentCreated(this, id);
        }
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::RemoveComponent(ComponentId id) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (!activeComponents[id]) return;
    scene->world->delayedActions.emplace_back([this, id]() {
        if (!activeComponents[id]) {
           return; // might have been removed by earlier remove action, eg if two consecutive RemoveComponent<> was called
        }
        if (scene->ComponentRemoved) {
            scene->ComponentRemoved(this, id);
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
    scene->world->delayedActions.emplace_back([this, id]() {
        if (scene->ComponentCreated) {
            scene->ComponentCreated(this, id);
        }
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::ReplaceComponent(ComponentId id, GameObject *referenceObject) {
    if (removed) return;
    scene->world->delayedActions.emplace_back([this, id, referenceObject]() {
        RemoveComponent(id);
        scene->world->delayedActions.emplace_back([this, id, referenceObject]() {
            AddComponent(id, referenceObject);
        });
    });
}

GameObject* GameObject::GetComponentOwner(ComponentId componentId) {
    int ownerIndex = scene->world->components[componentId].container->GetOwner(componentIndicies[componentId]);
    return ownerIndex != -1 ? &scene->world->objects.entries[ownerIndex] : nullptr;
}

void GameObject::SetWorldEnableDirty() {
    WorldEnabled.MakeDirty();
    scene->world->delayedActions.emplace_back([this](){
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

void GameObject::TryAddToSystem(int systemId) {
    if (systemId>=scene->systemsIndexed.size()) return; // systemindex is beyond scene's systems list, thus nothing else to do
    IGameSystem* system = scene->systemsIndexed[systemId];
    if (!system) return; // system is not part of scene/root
    GameWorld* world = scene->world;
    auto& systemBitset = world->systems[systemId].bitset;
    bool isInterest = systemBitset.Contains(enabledComponents);
    if (isInterest) {
        system->AddObject(this);
        system->ObjectAdded(this);
        if (system->ObjectCount() == 1) {
            world->AddActiveSystem(system, scene);
        }
    }
}

void GameObject::TryRemoveFromSystem(int systemId) {
    if (systemId>=scene->systemsIndexed.size()) return; // systemindex is beyond scene's systems list, thus nothing else to do
    IGameSystem* system = scene->systemsIndexed[systemId];
    if (!system) return; // system is not part of scene/root
    GameWorld* world = scene->world;
    auto& systemBitset = world->systems[systemId].bitset;
    bool wasInterest = systemBitset.Contains(enabledComponents);
    if (wasInterest) {
        system->ObjectRemoved(this);
        system->RemoveObject(this);
        if (system->ObjectCount() == 0) {
            world->RemoveActiveSystem(system);
        }
    }
}

void GameObject::TrySetComponentEnabled(ComponentId id, bool enable) {
    
    enable = enable && WorldEnabled();

    bool isEnabled = enabledComponents[id];
    if (isEnabled==enable) {
        return; //cannot double enable/disable components
    }

    GameWorld::ComponentInfo& componentInfo = scene->world->components[id];
    
    if (enable) {
        enabledComponents.Set(id, true);
        for(auto systemId : componentInfo.systemsUsingComponent) {
            TryAddToSystem(systemId);
        }
    } else {
        for(auto systemId : componentInfo.systemsUsingComponent) {
            TryRemoveFromSystem(systemId);
        }
        enabledComponents.Set(id, false);
    }
}

void GameObject::Remove() {
    if (removed) return;
    int localIndex = index;
    scene->world->delayedActions.emplace_back([this, localIndex]() {
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
        if (scene->ObjectRemoved) {
            scene->ObjectRemoved(this);
        }
    });
    removed = true;
    for(auto child : children) {
        child->Remove();
    }
}

bool GameObject::IsRemoved() const {
    return removed;
}

Handle<GameObject> GameObject::GetHandle() const {
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

GameObject* GameObject::CreateChildCloneInternal(std::vector<CloneReferenceComponent>& referenceComponents, GameObject* source, const std::function<bool(GameObject*)>& predicate) {
    if (predicate && !predicate(source)) return 0;
    
    GameObject* clone = CreateChild();
    
    for (int i=0; i<source->activeComponents.Size(); ++i) {
        if (source->activeComponents[i]) {
        
            int ownerIndex = scene->world->components[i].container->GetOwner(source->componentIndicies[i]);
            bool isReference = (ownerIndex != source->index);// && ownerIndex>=0;
            if (isReference) {
                referenceComponents.push_back({clone, i, ownerIndex});
            } else {
                clone->CloneComponent(i, source);
            }
        }
    }
    for(auto child : source->children) {
        clone->CreateChildCloneInternal(referenceComponents, child, predicate);
    }
    return clone;
}

GameObject* GameObject::CreateChildClone(Pocket::GameObject *source, const std::function<bool(GameObject*)>& predicate) {
    std::vector<CloneReferenceComponent> referenceComponents;
    GameObject* clone = CreateChildCloneInternal(referenceComponents, source, predicate);
    
    for(auto& referenceComponent : referenceComponents) {
        GameObject* referenceObject = (GameObject*)World()->objects.Get(referenceComponent.referenceObjectId);
        if (referenceObject->HasAncestor(source)) {
            int referenceIndex = 0;
            source->Recurse([&referenceIndex, &referenceObject] (GameObject* o) {
                if (o == referenceObject) {
                    return true;
                } else {
                    referenceIndex++;
                    return false;
                }
            });
        
            int indexCounter = 0;
            clone->Recurse([&referenceIndex, &indexCounter, &referenceObject] (GameObject* o) {
                if (indexCounter == referenceIndex) {
                    referenceObject = o;
                    return true;
                } else {
                    indexCounter++;
                    return false;
                }
            });
        }
        referenceComponent.object->AddComponent(referenceComponent.componentId, referenceObject);
    }
    
    return clone;
}

bool GameObject::Recurse(const std::function<bool(GameObject* object)>& function) {
    if (function(this)) return true;
    for(auto child : children) {
        if (child->Recurse(function)) return true;
    }
    return false;
}

GameObject* GameObject::CreateCopy(const std::function<bool(GameObject*)>& predicate) {
    if (IsRoot()) return 0;
    return Parent()->CreateChildClone(this, predicate);
}

GameObject* GameObject::Root() {
    return scene->root;
}

bool GameObject::IsRoot() const {
    return scene->root == this;
}

std::string& GameObject::RootGuid() const {
    return scene->guid;
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

std::vector<GameObject::ComponentEditor> GameObject::GetComponentEditors(const std::function<bool(int componentID)>& predicate) {
    std::vector<GameObject::ComponentEditor> editors;
    GameWorld* world = scene->world;
    for (int i=0; i<world->components.size(); ++i) {
        if (!activeComponents[i]) continue; // gameobject hasn't got component
        if (predicate && !predicate(i)) continue; // component type not allowed
        bool isReference = index != world->components[i].container->GetOwner(componentIndicies[i]);
        if (isReference) {
            TypeInfo type;
            type.name = world->components[i].name;
            GameObject::ReferenceComponent referenceComponent;
            referenceComponent.name = world->components[i].name;
            referenceComponent.componentId = i;
            referenceComponent.object = this;
            IFieldEditor* editor = FieldEditorCreator<GameObject::ReferenceComponent>::Create(&referenceComponent);
            editors.push_back({ type, editor });
            continue;
        }
        
        if (!world->components[i].getFieldEditor) {
            TypeInfo type;
            type.name = world->components[i].name;
            EmptyComponentEditor* emptyComponentEditor = new EmptyComponentEditor();
            emptyComponentEditor->SetField(&type);
            editors.push_back({ type, emptyComponentEditor });
            continue;
        } // component has no type
        IFieldEditor* editor = world->components[i].getFieldEditor(this);
        if (!editor) continue;
        TypeInfo info = world->components[i].getTypeInfo(this);
        info.name = world->components[i].name;
        editors.push_back({ info, editor });
    }
    return editors;
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

GameWorld* GameObject::World() {
    return scene->world;
}

TypeInfo GameObject::GetComponentTypeInfo(int index){
    GameWorld* world = scene->world;
    if (index>=world->components.size()) return TypeInfo();
    if (!activeComponents[index]) TypeInfo(); // gameobject hasn't got component
    if (!world->components[index].getTypeInfo) return TypeInfo(); // component has no type
    TypeInfo info = world->components[index].getTypeInfo(this);
    info.name = world->components[index].name;
    return info;
}

Property<bool>& GameObject::UpdateEnabled() { return scene->updateEnabled; }
Property<float>& GameObject::TimeScale() { return scene->timeScale; }
Property<bool>& GameObject::RenderEnabled() { return scene->renderEnabled; }

GameObject* GameObject::FindObject(int objectId) {
    return scene->FindObject(objectId);
}

std::string GameObject::TryGetRootPath() {
    return World()->TryFindScenePath(scene->guid);
}

void GameObject::SetCallbacks(const std::function<void (GameObject *)> &ObjectCreated, const std::function<void (GameObject *)> &ObjectRemoved, const std::function<void (GameObject *, ComponentId)> &ComponentCreated, const std::function<void (GameObject *, ComponentId)> &ComponentRemoved) {
    scene->ObjectCreated = ObjectCreated;
    scene->ObjectRemoved = ObjectRemoved;
    scene->ComponentCreated = ComponentCreated;
    scene->ComponentRemoved = ComponentRemoved;
}

bool GameObject::HasAncestor(Pocket::GameObject *ancestor) {
    GameObject* object = this;
    while (true) {
        if (object == ancestor) return true;
        object = object->Parent();
        if (!object) return false;
    }
}

GameObject* GameObject::Deserialize(const std::string &data) {
    GameObjectHandle handle = GameObjectHandle::Deserialize(data);
    return handle.operator->();
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

void GameObject::AddComponent(AddReferenceComponentList& addReferenceComponents, minijson::istream_context& context, std::string componentName) {
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

bool GameObject::GetAddReferenceComponent(AddReferenceComponentList& addReferenceComponents, Pocket::GameObject **object, int &componentID, GameObject** referenceObject) {
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
        
        if (!referenceScene) return false;
        
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



