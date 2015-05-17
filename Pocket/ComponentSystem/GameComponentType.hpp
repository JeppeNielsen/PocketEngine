//
//  GameComponentType.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 9/29/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <set>
#include "TypeDefs.hpp"
#include "GameObject.hpp"
#include "SerializedProperty.hpp"
#include <map>

namespace Pocket {
  
class GameObject;
class GameSystem;

class IGameComponentType {
public:
    IGameComponentType();
    virtual ~IGameComponentType();
    virtual IGameComponentType* Clone() = 0;
    
    std::string name;
    
    int id;
    ComponentMask mask;
    ComponentMask maskInverse;
    
    typedef std::vector<GameSystem*> Systems;
    Systems systems;
    
    bool isInEnabledChangedList;
    ObjectCollection changedObjects;
    
    bool isInRemovedList;
    typedef std::set<GameObject*> RemovedObjects;
    RemovedObjects removedObjects;
    
    void UpdateChangedObjects();
    void UpdateChangedObject(GameObject* object, bool enable);
    
    void UpdateRemovedObjects();
    virtual void UpdateRemovedObject(GameObject* object) = 0;
    virtual void* CloneComponent(void* source) = 0;
    virtual void AddReference(void* sourceComponent) = 0;
    virtual void StorePointerCounts(void* component, std::map<void*, int>& pointerCounter) = 0;
    virtual void UpdatePointers(void* component, std::map<void*, void*>& pointerMap, std::map<void*, int>& pointerCounter) = 0;
    virtual void* AddComponent() = 0;
    virtual void SerializeComponent(ISerializedProperty* serializedObject, void* component) = 0;
    virtual void DeserializeComponent(ISerializedProperty* serializedObject, void* component) = 0;
};

template<class T>
class GameComponent;

template<class T>
class GameComponentType : public IGameComponentType {
public:
    
    virtual ~GameComponentType();
    
    IGameComponentType* Clone();
    void* AddComponent();
    T* CloneComponent(T* sourceComponent);
    void* CloneComponent(void* source);
    void AddReference(void* source);
    void StorePointerCounts(void* component, std::map<void*, int>& pointerCounter);
    void UpdatePointers(void* component, std::map<void*, void*>& pointerMap, std::map<void*, int>& pointerCounter);
    void SerializeComponent(ISerializedProperty* serializedObject, void* component);
    void DeserializeComponent(ISerializedProperty* serializedObject, void* component);
    
    void UpdateRemovedObject(GameObject* object);
    
    void Trim();
    void Clear();
    
    static int AllocationSize;
    
    typedef std::vector<T*> GameComponentList;
    GameComponentList components;
    GameComponentList freeComponents;
};

}

template<class T>
int Pocket::GameComponentType<T>::AllocationSize = 16;

template<class T>
Pocket::IGameComponentType* Pocket::GameComponentType<T>::Clone() {
    GameComponentType<T>* clone = new GameComponentType<T>();
    clone->mask = mask;
    clone->maskInverse = maskInverse;
    clone->id = id;
    clone->name = name;
    return clone;
}

template<class T>
void* Pocket::GameComponentType<T>::AddComponent() {
    
    if (freeComponents.empty()) {
        for (int i=0; i<AllocationSize; i++) {
            T* component = new T();
            component->Reset();
            freeComponents.push_back(component);
        }
    }
    
    T* component = freeComponents.back();
    freeComponents.pop_back();
    component->indexInList = (int)components.size();
    component->referenceCount = 1;
    components.push_back(component);
    return component;
}

template<class T>
void Pocket::GameComponentType<T>::AddReference(void* source) {
    T* referenceComponent = (T*)source;
    referenceComponent->referenceCount++;
}

template<class T>
void Pocket::GameComponentType<T>::StorePointerCounts(void* c, std::map<void*, int>& pointerCounter) {
    T* component = (T*)c;
    pointerCounter[c]=(int)component->pointers.size();
}

template<class T>
void Pocket::GameComponentType<T>::UpdatePointers(void* c, std::map<void*, void*>& pointerMap, std::map<void*, int>& pointerCounter) {
    T* component = (T*)c;
    size_t count = component->pointers.size() - pointerCounter[c];
    for (int i=0; i<count; ++i) {
        Pointer<T>* pointer = component->pointers.back();
        std::map<void*, void*>::iterator it = pointerMap.find((void*)pointer->operator->());
        if (it != pointerMap.end()) {
            pointer->operator=((T*)it->second);
        }
    }
}

template<class T>
T* Pocket::GameComponentType<T>::CloneComponent(T* sourceComponent) {
    T* component = (T*)AddComponent();
    component->operator=(*sourceComponent);
    return component;
}

template<class T>
void* Pocket::GameComponentType<T>::CloneComponent(void* source) {
    return CloneComponent((T*)source);
}

template<class T>
void Pocket::GameComponentType<T>::UpdateRemovedObject(GameObject *object) {
    if ((object->activeComponents & mask) == mask) {
        UpdateChangedObject(object,false);
    }
    T* component = (T*)object->components[id];
    object->components[id] = 0;
    component->referenceCount--;
    
    if (component->referenceCount == 0) {
        size_t lastIndex = components.size() - 1;
        T* lastGameComponent = components[lastIndex];
        lastGameComponent->indexInList = component->indexInList;
        freeComponents.push_back(component);
        components[component->indexInList] = lastGameComponent;
        components.pop_back();
        component->Reset();
        component->ClearPointers();
    }
    object->ownedComponents &= maskInverse;
    object->enabledComponents &= maskInverse;
}

template<class T>
void Pocket::GameComponentType<T>::SerializeComponent(ISerializedProperty* serializedObject, void* component) {
    T* referenceComponent = (T*)component;
    referenceComponent->Serialize(serializedObject->Add(referenceComponent->Name(), referenceComponent));
}

template<class T>
void Pocket::GameComponentType<T>::DeserializeComponent(ISerializedProperty* serializedObject, void* component) {
    T* referenceComponent = (T*)component;
    ISerializedProperty* property = serializedObject->Get(referenceComponent->Name());
    if (property) {
        referenceComponent->Deserialize(property);
    }
}

template<class T>
void Pocket::GameComponentType<T>::Trim() {
    for (size_t i=0; i<freeComponents.size(); ++i) {
        delete freeComponents[i];
    }
    freeComponents.clear();
}

template<class T>
void Pocket::GameComponentType<T>::Clear() {
    Trim();
    for (size_t i=0; i<components.size(); ++i) {
        delete components[i];
    }
    components.clear();
}

template<class T>
Pocket::GameComponentType<T>::~GameComponentType() {
    Clear();
}

namespace Pocket {
 
class GameComponentTypeFactory {
public:
    static int componentIdCounter;
    
    typedef std::vector<IGameComponentType*> GameComponentTypes;
    
    static GameComponentTypes* createdComponentTypes;
    
    template<class T>
    static int GetID();
    
    static IGameComponentType* CreateType(int index);
};

}

template<class T>
int Pocket::GameComponentTypeFactory::GetID() {
    int id = componentIdCounter++;
    
    if (!createdComponentTypes) {
        createdComponentTypes = new GameComponentTypes();
    }
    
    if (id >= GameComponentTypeFactory::createdComponentTypes->size()) {
        GameComponentTypeFactory::createdComponentTypes->resize(id + 1);
        GameComponentType<T>* type = new GameComponentType<T>();
        type->mask = (ComponentMask)1<<id;
        type->maskInverse = (ComponentMask)~type->mask;
        type->id = id;
        type->name = T::NameStatic();
        GameComponentTypeFactory::createdComponentTypes->at(id) = type;
        
        for (int i=0; i<GameComponentTypeFactory::createdComponentTypes->size()-1; i++) {
            if (GameComponentTypeFactory::createdComponentTypes->at(i)->mask == type->mask) {
                //std::cout<<"Systems with same ids"<<std::endl;
            }
        }
        
    }
    return id;
}


