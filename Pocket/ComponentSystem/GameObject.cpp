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
#include "minijson_writer.hpp"

using namespace Pocket;

GameObject::GameObject() : Parent(this), Order(this), enabledComponents(0), activeComponents(0), ownedComponents(0), childIndex(0), components(0), systemIndicies(0) {
    Parent = 0;
    Parent.ChangedWithOld += Pocket::event_handler(this, &GameObject::ParentChanged);
    Order = 0;
}

GameObject::~GameObject() {
    delete [] components;
    delete [] systemIndicies;
}

void GameObject::ParentChanged(Pocket::Property<GameObject *, GameObject *>::EventData d) {
    ObjectCollection& oldChildren = d.Old ? d.Old->children : world->children;
    GameObject* lastChild = oldChildren.back();
    oldChildren[childIndex] = lastChild;
    lastChild->childIndex = childIndex;
    oldChildren.pop_back();
    ObjectCollection& children = d.Current ? d.Current->children : world->children;
    childIndex = (int)children.size();
    children.push_back(this);
    ToFront();
}

void GameObject::ToFront() {
    ObjectCollection& children = Parent() ? Parent()->children : world->children;
    if (children.size()<=1) return;
    int maxOrder = children[0]->Order();
    for (int i=1; i<children.size(); ++i) {
        int order = children[i]->Order;
        if (order>maxOrder) {
            maxOrder = order;
        }
    }
    Order = (maxOrder+1);
}

void GameObject::ToBack() {
    ObjectCollection& children = Parent() ? Parent()->children : world->children;
    if (children.size()<=1) return;
    int minOrder = children[0]->Order();
    for (int i=1; i<children.size(); ++i) {
        int order = children[i]->Order;
        if (order<minOrder) {
            minOrder = order;
        }
    }
    Order = (minOrder-1);
}

void GameObject::Initialize(size_t numberOfComponents) {
    components = new Components[numberOfComponents];
    for (int i=0; i<numberOfComponents; ++i) {
        components[i]=0;
    }
}

void GameObject::Remove() {
    if (isRemoved) return;
    isRemoved = true;
    world->removedObjects.push_back(this);
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

void GameObject::AddComponent(int componentType) {
    world->AddComponent(this, componentType);
}

GameWorld* GameObject::World() { return world; }

void GameObject::ToJson(std::ostream& stream) {
    
    minijson::writer_configuration config;
    config = config.pretty_printing(true);

    minijson::object_writer writer(stream, config);
    WriteJson(writer);
    writer.close();
}

void GameObject::WriteJson(minijson::object_writer& writer) {
    
    minijson::object_writer gameObject = writer.nested_object("GameObject");
    minijson::array_writer components = gameObject.nested_array("Components");
    
    for (int i=0; i<world->componentTypes.size(); i++) {
        if (this->components[i]) {
            world->WriteJsonComponent(components, this, i);
        }
    }
    components.close();
    
    if (!children.empty()) {
        minijson::array_writer children_object = gameObject.nested_array("Children");
        for(auto child : children) {
            minijson::object_writer child_object = children_object.nested_object();
            child->WriteJson(child_object);
            child_object.close();
        }
        children_object.close();
    }
    gameObject.close();
}

bool GameObject::IsComponentReference(int componentID) {
    ComponentMask mask = world->componentTypes[componentID]->mask;
    return !((ownedComponents & mask) == mask);
}

std::string GameObject::GetID() {
    std::string* id = world->FindIDFromObject(this);
    return id ? *id : "";
}

void GameObject::SetID(const std::string &id) {
    world->CreateObjectID(this, id);
}

SerializableCollection GameObject::SerializableComponents() {
    SerializableCollection collection;
     for (int i=0; i<world->componentTypes.size(); i++) {
        if (components[i]) {
            collection.push_back(world->componentTypes[i]->ConvertComponent(components[i]));
        }
    }
    return collection;
}

