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
    
    Parent.Changed.Bind(this, &GameObject::ParentChanged);
    
    WorldEnabled.Method = [this](bool& value) {
        value = (Parent) ? Parent()->WorldEnabled && Enabled : Enabled;
    }; 
}

void GameObject::ParentChanged() {
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
    scene->storage->components[id].container->Reference(referenceObject->componentIndicies[id]);
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
        scene->storage->components[id].container->Delete(componentIndicies[id], index);
        activeComponents.Set(id, false);
    });
}

void GameObject::CloneComponent(ComponentId id, GameObject* object) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (activeComponents[id]) return;
    if (!object->activeComponents[id]) return;
    componentIndicies[id] = scene->storage->components[id].container->Clone(object->componentIndicies[id], index);
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

void GameObject::EnableComponent(ComponentId id, bool enable) {
    scene->world->delayedActions.emplace_back([id, enable, this](){
        TrySetComponentEnabled(id, enable);
    });
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
    
    enable = enable && WorldEnabled();

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
    scene->world->delayedActions.emplace_back([this, localIndex]() {
        SetEnabled(false);
        forceSetNextParent = true;
        if (IsRoot()) {
            scene->world->RemoveRoot(this);
        } else {
            Parent = 0;
        }
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
    removed = true;
    for(auto child : children) {
        child->Remove();
    }
}

bool GameObject::IsRemoved() const {
    return removed;
}

int GameObject::RootId() const {
    return rootId;
}

GameObject* GameObject::CreateChild() {
    return scene->CreateEmptyObject(this, scene, true);
}

GameObject* GameObject::CreateObject() {
    return scene->root->CreateChild();
}

GameObject* GameObject::CreateChildCloneInternal(std::vector<CloneReferenceComponent>& referenceComponents, GameObject* source, const std::function<bool(GameObject*)>& predicate) {
    if (predicate && !predicate(source)) return 0;
    
    GameObject* clone = CreateChild();
    
    for (int i=0; i<source->activeComponents.Size(); ++i) {
        if (source->activeComponents[i]) {
        
            int ownerIndex = scene->storage->components[i].container->GetOwner(source->componentIndicies[i]);
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
        if (referenceComponent.referenceObjectId<0) {
            continue; // this reference object is removed
        }
        GameObject* referenceObject = (GameObject*)World()->storage->objects.Get(referenceComponent.referenceObjectId);
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
            clone->Recurse([&referenceIndex, &indexCounter, &referenceObject] (const GameObject* o) {
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
    
    return clone;
}

bool GameObject::Recurse(const std::function<bool(const GameObject* object)>& function) const {
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

GameObject* GameObject::Root() const {
    return scene->root;
}

bool GameObject::IsRoot() const {
    return scene->root == this;
}

std::string& GameObject::RootGuid() const {
    return scene->guid;
}

const ObjectCollection& GameObject::Children() const {
    return children;
}

int GameObject::ChildIndex() const {
    GameObject* parent = Parent();
    if (!parent) return 0;
    auto& children = parent->children;
    for(int i=0; i<children.size(); ++i) {
        if (children[i] == this) return i;
    }
    return 0;
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

Property<bool>& GameObject::UpdateEnabled() { return scene->updateEnabled; }
Property<float>& GameObject::TimeScale() { return scene->timeScale; }
Property<bool>& GameObject::RenderEnabled() { return scene->renderEnabled; }

GameObject* GameObject::FindObject(int objectId) const{
    return scene->FindObject(objectId);
}

std::string GameObject::TryGetRootPath() const {
    return World()->TryFindScenePath(scene->guid);
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
        object = object->Parent();
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




