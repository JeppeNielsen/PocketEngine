//
//  Container.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 29/12/15.
//  Copyright © 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <assert.h>

template<typename Object>
class Container {
public:
    struct ObjectInstance {
        Object object;
        int references;
        Container<Object>* owner;
    };
    
    ObjectInstance* CreateObject() {
        TryGrow();
        ObjectInstance* instance = objects[size++];
        ++instance->references;
        return instance;
    }
    
    int Size() const { return size; }
    
    void RemoveObject(ObjectInstance* instance) {
        if ((--instance->references) == 0) {
            if (instance->owner == this) {
                --size;
                std::swap(instance, objects[size]);
            } else {
                if (!instance->owner) {
                    delete instance;
                } else {
                    --instance->owner->size;
                    std::swap(instance, instance->owner->objects[instance->owner->size]);
                }
            }
        }
    }
    
    Object* Get(int index) {
        return &objects[index]->object;
    }
    
    void Clear() {
        for (int i=0; i<capacity; ++i) {
            if (objects[i]->references==0) {
                DeleteInstance(objects[i]);
            } else {
                objects[i]->owner = 0;
            }
        }
        size = 0;
        capacity = 0;
        objects.clear();
    }
    
private:
    
    void TryGrow() {
        if (size<capacity) return;
        Grow((capacity + 20) * 2);
    }

    void Grow(int newCapacity) {
        objects.resize(newCapacity);
        for (int i=size; i<newCapacity; ++i) {
            objects[i] = CreateInstance();
            objects[i]->references = 0;
            objects[i]->owner = this;
        }
        capacity = newCapacity;
    }
    
    using Objects = std::vector<ObjectInstance*>;
    Objects objects;
    
    int size;
    int capacity;
    
    ObjectInstance* CreateInstance() {
        return new ObjectInstance();
    }
    
    void DeleteInstance(ObjectInstance* instance) {
        delete instance;
    }
    
public:
    Container() : size(0), capacity(0) {}
    ~Container() {
        Clear();
    }
    
    void* createContext;
    void* deleteContext;
    int contextIndex;
};