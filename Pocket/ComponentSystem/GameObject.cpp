//
//  GameObject.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 9/25/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "GameObject.hpp"
#include "GameComponent.hpp"
#include "GameWorld.hpp"

using namespace Pocket;

GameObject::GameObject() : Parent(this), enabledComponents(0), activeComponents(0), ownedComponents(0) {
    Parent = 0;
    Parent.ChangedWithOld += Pocket::event_handler(this, &GameObject::ParentChanged);
}

GameObject::~GameObject() {
    delete [] components;
    delete [] systemIndicies;
}

void GameObject::ParentChanged(Pocket::Property<GameObject *, GameObject *>::EventData d) {
    if (d.Old) {
        ObjectCollection::iterator it = std::find(d.Old->children.begin(), d.Old->children.end(), this);
        d.Old->children.erase(it);
    } else {
        ObjectCollection::iterator it = std::find(world->children.begin(), world->children.end(), this);
        world->children.erase(it);
    }
    if (d.Current) {
        d.Current->children.push_back(this);
    } else {
        world->children.push_back(this);
    }
}

void GameObject::ToFront() {
    ObjectCollection& children = Parent() ? Parent()->children : world->children;
    ObjectCollection::iterator it = std::find(children.begin(), children.end(), this);
    children.erase(it);
    children.push_back(this);
    OrderChanged(this);
}

void GameObject::ToBack() {
    ObjectCollection& children = Parent() ? Parent()->children : world->children;
    ObjectCollection::iterator it = std::find(children.begin(), children.end(), this);
    children.erase(it);
    children.insert(children.begin(), this);
    OrderChanged(this);
}

int GameObject::ChildIndex() {
    ObjectCollection& children = Parent() ? Parent()->children : world->activeObjects;
    ObjectCollection::iterator it = std::find(children.begin(), children.end(), this);
    return (int)(it - children.begin());
}

void GameObject::Initialize(size_t numberOfComponents) {
    components = new Components[numberOfComponents];
    for (int i=0; i<numberOfComponents; ++i) {
        components[i]=0;
    }
}

void GameObject::Remove() {
    world->removedObjects.insert(this);
    for (size_t i=0; i<children.size(); i++) {
        children[i]->Remove();
    }
}

const ObjectCollection& GameObject::Children() { return children; }

GameObject* GameObject::Clone() {
    return Clone(Parent);
}

GameObject* GameObject::Clone(GameObject* parent) {
    world->pointerMap.clear();
    world->pointerCounter.clear();
    CountPointers();
    GameObject* clone = CloneInternal(parent);
    UpdatePointers();
    return clone;
}

void GameObject::CountPointers() {
    world->pointerCounter[this] = (int)pointers.size();
    for (size_t i=0; i<world->componentTypes.size(); i++) {
        if (components[i]) {
            world->componentTypes[i]->StorePointerCounts(components[i], world->pointerCounter);
        }
    }
    for (size_t i=0; i<children.size(); i++) {
        children[i]->CountPointers();
    }
}

GameObject* GameObject::CloneInternal(GameObject* parent) {
    GameObject* clone = world->CreateObject();
    world->pointerMap[this] = clone;
    clone->Parent = parent;
    for (size_t i=0; i<world->componentTypes.size(); i++) {
        if (components[i]) {
            void* clonedComponent = world->CloneComponent(clone, (int)i, this);
            world->pointerMap[components[i]] = clonedComponent;
        }
    }
    for (size_t i=0; i<children.size(); i++) {
        children[i]->CloneInternal(clone);
    }
    
    return clone;
}

void GameObject::UpdatePointers() {
    size_t count = pointers.size() - world->pointerCounter[this];
    for (int i=0; i<count; ++i) {
        Pointer<GameObject>* pointer = pointers.back();
        GameWorld::PointerMap::iterator it = world->pointerMap.find((void*)pointer->operator->());
        if (it!=world->pointerMap.end()) {
            pointer->operator=((GameObject*)it->second);
        }
    }
    for (size_t i=0; i<world->componentTypes.size(); i++) {
        if (components[i]) {
            world->UpdatePointers(this, (int)i);
        }
    }
    
    for (size_t i=0; i<children.size(); i++) {
        children[i]->UpdatePointers();
    }
}

void GameObject::Serialize(ISerializedProperty* serializedObject) {
    for (size_t i=0; i<world->componentTypes.size(); i++) {
        if (this->components[i]) {
            world->SerializeComponent(serializedObject, this, (int)i);
        }
    }
}

void GameObject::Deserialize(ISerializedProperty* serializedObject) {
    for (size_t i=0; i<world->componentTypes.size(); i++) {
        if (this->components[i]) {
            world->DeserializeComponent(serializedObject, this, (int)i);
        }
    }
}

void GameObject::AddComponent(int componentType) {
    world->AddComponent(this, componentType);
}

GameWorld* GameObject::World() { return world; }



