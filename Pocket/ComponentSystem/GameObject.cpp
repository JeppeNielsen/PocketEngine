//
//  GameObject.cpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 06/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameObject.hpp"
#include <assert.h>
#include "GameWorld.hpp"

using namespace Pocket;

static bool forceSetNextParent = false;

const ObjectCollection& GameObject::Children() const { return data->children; }
Property<GameObject*>& GameObject::Parent() { return data->Parent; }
Property<bool>& GameObject::Enabled() { return data->Enabled; }
DirtyProperty<bool>& GameObject::WorldEnabled() { return data->WorldEnabled; }
Property<int>& GameObject::Order() { return data->Order; }

GameObject::GameObject()
    :
    world(0), index(0), data(0)
{
    data = new Data();

    data->removed = false;
    data->Enabled = true;
    data->Parent = 0;
    data->Order = 0;
    
    data->Parent.Changed.Bind([this]() {
        assert(data->Parent!=this);
        GameObject* prevParent = data->Parent.PreviousValue();
        GameObject* currentParent = data->Parent;
        
        if (data->removed && !forceSetNextParent) return;
        
        if (!prevParent) {
            prevParent = &world->root;
        }
        if (!currentParent && !forceSetNextParent) {
            currentParent = &world->root;
        }
        
        if (prevParent) {
            auto& children = prevParent->data->children;
            children.erase(std::find(children.begin(), children.end(), this));
            prevParent->data->WorldEnabled.HasBecomeDirty.Unbind(this, &GameObject::SetWorldEnableDirty);
        }
        
        if (currentParent) {
            auto& children = currentParent->data->children;
            children.push_back(this);
            currentParent->data->WorldEnabled.HasBecomeDirty.Bind(this, &GameObject::SetWorldEnableDirty);
            
            bool prevWorldEnabled = data->WorldEnabled;
            data->WorldEnabled.MakeDirty();
            if (data->WorldEnabled()!=prevWorldEnabled) {
                SetWorldEnableDirty();
            }
        }
    });
    
    data->WorldEnabled.Method = [this](bool& value) {
        value = (data->Parent) ? data->Parent()->data->WorldEnabled && data->Enabled : data->Enabled;
    };
    
    data->Enabled.Changed.Bind(this, &GameObject::SetWorldEnableDirty);
}

GameObject::~GameObject() {
    delete data;
}

bool GameObject::HasComponent(ComponentID id) const {
    assert(id<world->numComponentTypes);
    return data->activeComponents[id];
}

void* GameObject::GetComponent(ComponentID id) {
    assert(id<world->numComponentTypes);
    if (!data->activeComponents[id]) return 0;
    auto& objectComponent = world->objectComponents[id][index];
    return objectComponent.container->Get(objectComponent.index);
}

void GameObject::AddComponent(ComponentID id) {
    assert(id<world->numComponentTypes);
    if (HasComponent(id)) {
        return;
    }
    if (data->removed) return;
    IContainer* container = world->components[id];
    world->objectComponents[id][index] = { container->Create(), container };
    data->activeComponents.Set(id, true);
    world->delayedActions.emplace_back([this, id]() {
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::AddComponent(ComponentID id, GameObject* source) {
    assert(id<world->numComponentTypes);
    assert(source->HasComponent(id));
    if (HasComponent(id)) {
        return;
    }
    if (data->removed) return;
    auto& sourceObjectComponent = source->world->objectComponents[id][source->index];
    world->objectComponents[id][index] = sourceObjectComponent;
    sourceObjectComponent.container->Reference(sourceObjectComponent.index);
    data->activeComponents.Set(id, true);
    world->delayedActions.emplace_back([this, id]() {
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::CloneComponent(ComponentID id, GameObject* source) {
    assert(id<world->numComponentTypes);
    assert(source->HasComponent(id));
    if (HasComponent(id)) {
        return;
    }
    if (data->removed) return;
    auto& sourceObjectComponent = source->world->objectComponents[id][source->index];
    IContainer* container = world->components[id];
    int componentIndex = 0;
    if (container == sourceObjectComponent.container) {
        componentIndex = container->Clone(sourceObjectComponent.index);
    } else {
        componentIndex = container->Clone(sourceObjectComponent.container->Get(sourceObjectComponent.index));
    }
    world->objectComponents[id][index] = { componentIndex, container };
    data->activeComponents.Set(id, true);
    world->delayedActions.emplace_back([this, id]() {
        TrySetComponentEnabled(id, true);
    });
}

void GameObject::RemoveComponent(ComponentID id) {
    if (id>=world->numComponentTypes) return;
    if (!HasComponent(id)) {
        return;
    }
    if (data->removed) return;
    world->delayedActions.emplace_back([this, id]() {
        if (!data->activeComponents[id]) {
           return; // might have been removed by earlier remove action, eg if two consecutive RemoveComponent<> was called
        }
        TrySetComponentEnabled(id, false);
        auto& objectComponent = world->objectComponents[id][index];
        objectComponent.container->Delete(objectComponent.index);
        world->objectComponents[id][index].index = -1;
        data->activeComponents.Set(id, false);
    });
}

void GameObject::Remove() {
    if (data->removed) return;
    int localIndex = index;
    world->delayedActions.emplace_back([this, localIndex]() {
        forceSetNextParent = true;
        if (data->Parent()) {
            data->Parent = 0;
        } else {
            auto& children = world->root.data->children;
            children.erase(std::find(children.begin(), children.end(), this));
        }
        forceSetNextParent = false;
        SetEnabled(false);
        world->objectsFreeIndicies.push_back(localIndex);
        --world->objectCount;
    });
    data->removed = true;
    for(auto child : data->children) {
        child->Remove();
    }
}

bool GameObject::IsRemoved() {
    return data->removed;
}

GameObject* GameObject::Clone() {
    GameObject* clone = world->CreateObject();
    for(int i=0; i<world->numComponentTypes; ++i) {
        if (data->activeComponents[i]) {
            clone->CloneComponent(i, this);
        }
    }
    for(auto child : data->children) {
        auto childClone = child->Clone();
        childClone->Parent() = clone;
    }
    return clone;
}

std::vector<TypeInfo> GameObject::GetComponentTypes(std::function<bool(int componentID)> predicate) {
    std::vector<TypeInfo> infos;
    for (int i=0; i<world->components.size(); ++i) {
        if (!world->componentInfos[i].getTypeInfo) continue; // component has no type
        if (!data->activeComponents[i]) continue; // gameobject hasn't got component
        if (predicate && !predicate(i)) continue; // component type not allowed
        infos.emplace_back(world->componentInfos[i].getTypeInfo(this));
        infos[infos.size()-1].name = world->componentInfos[i].name;
    }
    return infos;
}

void GameObject::TryAddComponentContainer(ComponentID id, std::function<IContainer *(GameObject::ComponentInfo&)>&& constructor) {
    world->TryAddComponentContainer(id, std::move(constructor));
}

void GameObject::SetWorldEnableDirty() {
    data->WorldEnabled.MakeDirty();
    world->delayedActions.emplace_back([this](){
        SetEnabled(data->WorldEnabled);
    });
}

void GameObject::SetEnabled(bool enabled) {
    for(int i=0; i<world->numComponentTypes; ++i) {
        if (data->activeComponents[i]) {
            TrySetComponentEnabled(i, enabled);
        }
    }
}

void GameObject::TrySetComponentEnabled(ComponentID id, bool enable) {
    
    enable = enable && WorldEnabled();

    bool isEnabled = data->enabledComponents[id];
    if (isEnabled==enable) {
        return; //cannot double enable/disable components
    }
    
    if (enable) {
        data->enabledComponents.Set(id, true);
        if (id>=world->systemsPerComponent.size()) return; // component id is beyond systems
        auto& systemsUsingComponent = world->systemsPerComponent[id];
        for(auto systemIndex : systemsUsingComponent) {
            auto& bitset = world->systemBitsets[systemIndex];
            bool isInterest = bitset.Contains(data->enabledComponents);
            if (isInterest) {
                auto* system = world->systems[systemIndex];
                system->AddObject(this);
                system->ObjectAdded(this);
            }
        }
    } else {
        if (id>=world->systemsPerComponent.size()) return; // component id is beyond systems
        auto& systemsUsingComponent = world->systemsPerComponent[id];
        for(auto systemIndex : systemsUsingComponent) {
            auto& bitset = world->systemBitsets[systemIndex];
            bool wasInterest = bitset.Contains(data->enabledComponents);
            if (wasInterest) {
                auto* system = world->systems[systemIndex];
                system->ObjectRemoved(this);
                system->RemoveObject(this);
            }
        }
        data->enabledComponents.Set(id, false);
    }
}
