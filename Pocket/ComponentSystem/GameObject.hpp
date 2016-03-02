//
//  GameObject.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 27/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "assert.h"
#include <vector>
#include "Property.hpp"
#include "IDHelper.hpp"

class GameObject;

using ObjectCollection = std::vector<GameObject*>;

class IGameWorld;

class GameObject {
protected:
    
    friend class IGameWorld;

    struct Command {
        std::function<void*(GameObject*)> addComponent;
        std::function<void*(GameObject*, GameObject*)> addComponentReference;
        std::function<void(GameObject*)> removeComponent;
    };
    
    using Commands = std::vector<Command>;

    using Component = void*;
    Component* components;

    GameObject() {
        int numberOfComponents = IDHelper::NumberOfComponents();
        components = new Component[numberOfComponents];
        for(int i=0; i<numberOfComponents; ++i) {
            components[i] = 0;
        }
        
        Parent = 0;
        Parent.Changed.Bind([this]() {
            assert(Parent!=this);
            auto prevParent = Parent.PreviousValue();
            if (prevParent) {
                auto& children = prevParent->children;
                children.erase(std::find(children.begin(), children.end(), this));
            }
            
            if (Parent) {
                Parent()->children.push_back(this);
            }
        });
    }
    virtual ~GameObject() { delete[] components; }
    
    Commands* commands;
    ObjectCollection children;

public:
    template<typename T>
    T* GetComponent() {
        return (T*)components[IDHelper::GetComponentID<T>()];
    }

    template<typename T>
    T* AddComponent() {
        return (T*)commands->operator[](IDHelper::GetComponentID<T>()).addComponent(this);
    }
    
    template<typename T>
    T* AddComponent(GameObject* source) {
        return (T*)commands->operator[](IDHelper::GetComponentID<T>()).addComponentReference(this, source);
    }
    
    template<typename T>
    void RemoveComponent() {
        commands->operator[](IDHelper::GetComponentID<T>()).removeComponent(this);
    }
    
    Pocket::Property<GameObject*> Parent;
    const ObjectCollection& Children() { return children; }
    
    Pocket::Property<int> Order;
    
    virtual void Remove() = 0;
    
    using SerializePredicate = std::function<bool(GameObject*, int)>;
    
    virtual void ToJson(std::ostream& stream, SerializePredicate predicate = 0) = 0;
};