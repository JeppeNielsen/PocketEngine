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

bool GameObject::HasComponent(ComponentId id) {
    assert(id<activeComponents.Size());
    return activeComponents[id];
}

void* GameObject::GetComponent(ComponentId id) {
    assert(id<activeComponents.Size());
    if (!activeComponents[id]) return 0;
    return scene->world->components[id].container->Get(componentIndicies[id]);
}

void GameObject::AddComponent(ComponentId id) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (activeComponents[id]) return;

    componentIndicies[id] = scene->world->components[id].container->Create();
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
        scene->world->components[id].container->Delete(componentIndicies[id]);
        activeComponents.Set(id, false);
    });
}

void GameObject::CloneComponent(ComponentId id, GameObject* object) {
    assert(id<activeComponents.Size());
    if (removed) return;
    if (activeComponents[id]) return;
    if (!object->activeComponents[id]) return;
    componentIndicies[id] = scene->world->components[id].container->Clone(object->componentIndicies[id]);
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
            auto& systemBitset = world->systems[systemIndex].bitset;
            bool isInterest = systemBitset.Contains(enabledComponents);
            if (isInterest) {
                IGameSystem* system = scene->systemsIndexed[systemIndex];
                system->AddObject(this);
                system->ObjectAdded(this);
                if (system->ObjectCount() == 1) {
                    scene->activeSystems.push_back(system);
                }
            }
        }
    } else {
        for(auto systemIndex : componentInfo.systemsUsingComponent) {
            auto& systemBitset = world->systems[systemIndex].bitset;
            bool wasInterest = systemBitset.Contains(enabledComponents);
            if (wasInterest) {
                IGameSystem* system = scene->systemsIndexed[systemIndex];
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
        scene->world->objects.Delete(index);
    });
    removed = true;
    for(auto child : children) {
        child->Remove();
    }
}

bool GameObject::IsRemoved() {
    return removed;
}

Handle<GameObject> GameObject::GetHandle() {
    return scene->world->objects.GetHandle(index);
}

GameObject* GameObject::CreateChild() {
    auto& worldObjects = scene->world->objects;
    int index = worldObjects.CreateNoInit();
    GameObject* object = &worldObjects.entries[index];
    object->scene = scene;
    object->index = index;
    object->Reset();
    object->Parent = this;
    return object;
}

GameObject* GameObject::CreateObject() {
    return scene->root->CreateChild();
}

GameObject* GameObject::Root() {
    return scene->root;
}

bool GameObject::IsRoot() {
    return scene->root == this;
}

const ObjectCollection& GameObject::Children() {
    return children;
}

IGameSystem* GameObject::GetSystem(SystemId id) {
    return scene->systemsIndexed[id];
}