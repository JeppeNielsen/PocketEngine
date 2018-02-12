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
#include "GameStorage.hpp"
#include "Hierarchy.hpp"

using namespace Pocket;

static bool forceSetNextParent = false;

GameObject::GameObject() { }

GameObject::~GameObject() { }

GameObject::GameObject(const GameObject& other) {
    activeComponents.Resize(other.activeComponents.Size());
    enabledComponents.Resize(other.activeComponents.Size());
    componentIndicies.resize(other.activeComponents.Size());
}

void GameObject::Reset() {
    removed = false;
    ComponentId id = GameIdHelper::GetComponentID<class Hierarchy>();
    auto& container = scene->storage->components[id].container;
    componentIndicies[id] = container->Create(index);
    activeComponents.Set(id, true);
    class Hierarchy* h = (class Hierarchy*)container->Get(componentIndicies[id]);
    h->owner = this;
}

bool GameObject::HasComponent(ComponentId id) const {
    if (id>=activeComponents.Size()) {
        return false;
    }
    return activeComponents[id];
}

void* GameObject::GetComponent(ComponentId id) const {
    if (id>=activeComponents.Size()) {
        return nullptr;
    }
    if (!activeComponents[id]) return nullptr;
    return scene->storage->components[id].container->Get(componentIndicies[id]);
}

void GameObject::AddComponent(ComponentId id) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (activeComponents[id]) return;

    componentIndicies[id] = scene->storage->components[id].container->Create(index);
    activeComponents.Set(id, true);
    if (scene->world) {
        scene->world->delayedActions.emplace_back([this, id]() {
            if (scene->ComponentCreated) {
                scene->ComponentCreated(this, id);
            }
            TrySetComponentEnabled(id, true);
        });
    }
}

void GameObject::AddComponent(ComponentId id, GameObject* referenceObject) {
    assert(id<activeComponents.Size());
    assert(scene->storage == referenceObject->scene->storage);
    if (removed) return;
    if (activeComponents[id]) return;
    if (!referenceObject->activeComponents[id]) return;
    
    componentIndicies[id] = referenceObject->componentIndicies[id];
    scene->storage->components[id].container->Reference(referenceObject->componentIndicies[id]);
    activeComponents.Set(id, true);
    if (scene->world) {
        scene->world->delayedActions.emplace_back([this, id]() {
            if (scene->ComponentCreated) {
                scene->ComponentCreated(this, id);
            }
            TrySetComponentEnabled(id, true);
        });
    }
}

void GameObject::RemoveComponent(ComponentId id) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (!activeComponents[id]) return;
    
    if (scene->world) {
        scene->world->delayedActions.emplace_back([this, id]() {
            if (!activeComponents[id]) {
               return; // might have been removed by earlier remove action, eg if two consecutive RemoveComponent<> was called
            }
            if (scene->ComponentRemoved) {
                scene->ComponentRemoved(this, id);
            }
            TrySetComponentEnabled(id, false);
            scene->storage->components[id].container->Delete(componentIndicies[id], index);
            activeComponents.Set(id, false);
        });
    } else {
        scene->storage->components[id].container->Delete(componentIndicies[id], index);
        activeComponents.Set(id, false);
    }
}

void GameObject::CloneComponent(ComponentId id, GameObject* object) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (activeComponents[id]) return;
    if (!object->activeComponents[id]) return;
    componentIndicies[id] = scene->storage->components[id].container->Clone(object->componentIndicies[id], index);
    activeComponents.Set(id, true);
    
    if (scene->world) {
        scene->world->delayedActions.emplace_back([this, id]() {
            if (scene->ComponentCreated) {
                scene->ComponentCreated(this, id);
            }
            TrySetComponentEnabled(id, true);
        });
    }
}

void GameObject::ReplaceComponent(ComponentId id, GameObject *referenceObject) {
    if (removed) return;
    
    if (scene->world) {
        scene->world->delayedActions.emplace_back([this, id, referenceObject]() {
            RemoveComponent(id);
            scene->world->delayedActions.emplace_back([this, id, referenceObject]() {
                AddComponent(id, referenceObject);
            });
        });
    }
}

GameObject* GameObject::GetComponentOwner(ComponentId componentId) const {
    int ownerIndex = scene->storage->components[componentId].container->GetOwner(componentIndicies[componentId]);
    return ownerIndex != -1 ? &scene->storage->objects.entries[ownerIndex] : nullptr;
}

int GameObject::ComponentCount() const {
    int componentCounter = 0;
    for(int i=0; i<activeComponents.Size(); ++i) {
        if (activeComponents[i]) {
            componentCounter++;
        }
    }
    return componentCounter;
}

void GameObject::SetWorldEnableDirty() {
    /*WorldEnabled.MakeDirty();
    if (scene->world) {
        scene->world->delayedActions.emplace_back([this](){
            SetEnabled(WorldEnabled);
        });
    }
    */
}


void GameObject::SetEnabled(bool enabled) {
    for(int i=0; i<activeComponents.Size(); ++i) {
        if (activeComponents[i]) {
            TrySetComponentEnabled(i, enabled);
        }
    }
}

void GameObject::EnableComponent(ComponentId id, bool enable) {
    if (scene->world) {
        scene->world->delayedActions.emplace_back([id, enable, this](){
            TrySetComponentEnabled(id, enable);
        });
    }
}

void GameObject::TryAddToSystem(int systemId) {
    if (systemId>=scene->systemsIndexed.size()) return; // systemindex is beyond scene's systems list, thus nothing else to do
    IGameSystem* system = scene->systemsIndexed[systemId];
    if (!system) return; // system is not part of scene/root
    GameWorld* world = scene->world;
    auto& systemBitset = scene->storage->systems[systemId].bitset;
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
    auto& systemBitset = scene->storage->systems[systemId].bitset;
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
    
    //enable = enable && WorldEnabled();

    bool isEnabled = enabledComponents[id];
    if (isEnabled==enable) {
        return; //cannot double enable/disable components
    }

    auto& componentInfo = scene->storage->components[id];
    
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
    if (scene->world) {
        scene->world->delayedActions.emplace_back([this, localIndex]() {
            SetEnabled(false);
            forceSetNextParent = true;
            if (IsRoot()) {
                scene->world->RemoveScene(this);
            }
            Hierarchy().Parent = nullptr;
            forceSetNextParent = false;
            for(int i=0; i<activeComponents.Size(); ++i) {
                if (activeComponents[i]) {
                    scene->storage->components[i].container->Delete(componentIndicies[i], index);
                    activeComponents.Set(i, false);
                }
            }
            scene->storage->objects.Delete(index, 0);
            if (scene->ObjectRemoved) {
                scene->ObjectRemoved(this);
            }
        });
    } else {
        for(int i=0; i<activeComponents.Size(); ++i) {
            if (activeComponents[i]) {
                scene->storage->components[i].container->Delete(componentIndicies[i], index);
                activeComponents.Set(i, false);
            }
        }
        scene->storage->objects.Delete(index, 0);
    }
    removed = true;
    class Hierarchy& h = Hierarchy();
    for(auto child : h.children) {
        child->Remove();
    }
}

bool GameObject::IsRemoved() const {
    return removed;
}

int GameObject::RootId() const {
    return id;
}

GameObject* GameObject::CreateChild() {
    return scene->CreateEmptyObject(this, true);
}

GameObject* GameObject::CreateObject() {
    return scene->root->CreateChild();
}

void GameObject::ApplyCloneInternal(std::vector<CloneReferenceComponent>& referenceComponents, GameObject* source, const std::function<bool(GameObject*)>& predicate) {
    if (predicate && !predicate(source)) return;
    
    for (int i=0; i<source->activeComponents.Size(); ++i) {
        if (source->activeComponents[i]) {
            int ownerIndex = scene->storage->components[i].container->GetOwner(source->componentIndicies[i]);
            bool isReference = (ownerIndex != source->index);// && ownerIndex>=0;
            if (isReference) {
                referenceComponents.push_back({this, i, ownerIndex});
            } else {
                this->CloneComponent(i, source);
            }
        }
    }
    class Hierarchy& h = source->Hierarchy();
    for(auto child : h.children) {
        GameObject* childClone = CreateChild();
        childClone->ApplyCloneInternal(referenceComponents, child, predicate);
    }
}

void GameObject::ApplyClone(Pocket::GameObject *source, const std::function<bool(GameObject*)>& predicate) {
    std::vector<CloneReferenceComponent> referenceComponents;
    ApplyCloneInternal(referenceComponents, source, predicate);
    
    for(auto& referenceComponent : referenceComponents) {
        if (referenceComponent.referenceObjectId<0) {
            continue; // this reference object is removed
        }
        GameObject* referenceObject = (GameObject*)scene->storage->objects.Get(referenceComponent.referenceObjectId);
        if (referenceObject->HasAncestor(source)) {
            int referenceIndex = 0;
            source->Recurse([&referenceIndex, &referenceObject] (const GameObject* o) {
                if (o == referenceObject) {
                    return true;
                } else {
                    referenceIndex++;
                    return false;
                }
            });
        
            int indexCounter = 0;
            this->Recurse([&referenceIndex, &indexCounter, &referenceObject] (const GameObject* o) {
                if (indexCounter == referenceIndex) {
                    referenceObject = (GameObject*)o;
                    return true;
                } else {
                    indexCounter++;
                    return false;
                }
            });
        }
        referenceComponent.object->AddComponent(referenceComponent.componentId, referenceObject);
    }
}

GameObject* GameObject::CreateChildClone(GameObject* source, const std::function<bool(GameObject*)>& predicate) {
    GameObject* child = CreateChild();
    child->ApplyClone(source, predicate);
    return child;
}

bool GameObject::Recurse(const std::function<bool(const GameObject* object)>& function) const {
    if (function(this)) return true;
    class Hierarchy& h = Hierarchy();
    for(auto child : h.children) {
        if (child->Recurse(function)) return true;
    }
    return false;
}

GameObject* GameObject::CreateCopy(const std::function<bool(GameObject*)>& predicate) {
    if (IsRoot()) return nullptr;
    return Hierarchy().Parent()->CreateChildClone(this, predicate);
}

GameObject* GameObject::Root() const {
    return scene->root;
}

bool GameObject::IsRoot() const {
    return scene->root == this;
}

std::string& GameObject::RootGuid() const {
    return scene->guid;
}

InputManager& GameObject::Input() const { return scene->world->Input(); }

std::vector<TypeInfo> GameObject::GetComponentTypes(const std::function<bool(int componentID)>& predicate) const {
    std::vector<TypeInfo> infos;
    GameStorage* storage = scene->storage;
    for (int i=0; i<storage->components.size(); ++i) {
        if (!storage->components[i].getTypeInfo) continue; // component has no type
        if (!activeComponents[i]) continue; // gameobject hasn't got component
        if (predicate && !predicate(i)) continue; // component type not allowed
        infos.emplace_back(storage->components[i].getTypeInfo(this));
        infos[infos.size()-1].name = storage->components[i].name;
    }
    return infos;
}

std::vector<GameObject::ComponentEditor> GameObject::GetComponentEditors(const std::function<bool(int componentID)>& predicate) const {
    std::vector<GameObject::ComponentEditor> editors;
    GameStorage* storage = scene->storage;
    for (int i=0; i<storage->components.size(); ++i) {
        if (!activeComponents[i]) continue; // gameobject hasn't got component
        if (predicate && !predicate(i)) continue; // component type not allowed
        bool isReference = index != storage->components[i].container->GetOwner(componentIndicies[i]);
        if (isReference) {
            TypeInfo type;
            type.name = storage->components[i].name;
            GameObject::ReferenceComponent referenceComponent;
            referenceComponent.name = storage->components[i].name;
            referenceComponent.componentId = i;
            referenceComponent.object = (GameObject*)this;
            IFieldEditor* editor = FieldEditorCreator<GameObject::ReferenceComponent>::Create(&referenceComponent);
            editors.push_back({ type, editor });
            continue;
        }
        
        if (!storage->components[i].getFieldEditor) {
            TypeInfo type;
            type.name = storage->components[i].name;
            EmptyComponentEditor* emptyComponentEditor = new EmptyComponentEditor();
            emptyComponentEditor->SetField(&type);
            editors.push_back({ type, emptyComponentEditor });
            continue;
        } // component has no type
        IFieldEditor* editor = storage->components[i].getFieldEditor(this);
        if (!editor) continue;
        TypeInfo info = storage->components[i].getTypeInfo(this);
        info.name = storage->components[i].name;
        editors.push_back({ info, editor });
    }
    return editors;
}

std::vector<int> GameObject::GetComponentIndicies() const {
    std::vector<int> indicies;
    for (int i=0; i<activeComponents.Size(); ++i) {
        if (activeComponents[i]) {
            indicies.push_back(i);
        }
    }
    return indicies;
}

GameWorld* GameObject::World() const {
    return scene->world;
}

TypeInfo GameObject::GetComponentTypeInfo(int index) const {
    GameStorage* storage = scene->storage;
    if (index>=storage->components.size()) return TypeInfo();
    if (!activeComponents[index]) TypeInfo(); // gameobject hasn't got component
    if (!storage->components[index].getTypeInfo) return TypeInfo(); // component has no type
    TypeInfo info = storage->components[index].getTypeInfo(this);
    info.name = storage->components[index].name;
    return info;
}

void GameObject::SetCallbacks(const std::function<void (GameObject *)> &ObjectCreated, const std::function<void (GameObject *)> &ObjectRemoved, const std::function<void (GameObject *, ComponentId)> &ComponentCreated, const std::function<void (GameObject *, ComponentId)> &ComponentRemoved) const {
    scene->ObjectCreated = ObjectCreated;
    scene->ObjectRemoved = ObjectRemoved;
    scene->ComponentCreated = ComponentCreated;
    scene->ComponentRemoved = ComponentRemoved;
}

bool GameObject::HasAncestor(Pocket::GameObject *ancestor) const {
    const GameObject* object = this;
    while (true) {
        if (object == ancestor) return true;
        object = object->Hierarchy().Parent;
        if (!object) return false;
    }
}

GameObject* GameObject::Deserialize(const std::string &data) {
    GameObjectHandle handle = GameObjectHandle::Deserialize(data);
    return handle.operator->();
}

void GameObject::RemoveComponents(const SerializePredicate& predicate) {
    for(int i=0; i<scene->storage->components.size(); ++i) {
        if (activeComponents[i] && !(predicate && !predicate(this, i))) {
            RemoveComponent(i);
        }
    }
}

class Hierarchy& GameObject::Hierarchy() const {
    return *GetComponent<class Hierarchy>();
}

std::string GameObject::TryGetScenePath() const {
    return scene->storage->GuidToPath(scene->guid);
}


