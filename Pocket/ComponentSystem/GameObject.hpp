//
//  GameObject.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 9/25/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "Property.hpp"
#include "TypeDefs.hpp"
#include "IPointable.hpp"

namespace Pocket {

class GameWorld;
class GameObject;
class IGameComponentType;
template<class T>
class GameComponentType;
class ScriptWorld;

class GameObject : public IPointable<GameObject> {
public:
    Pocket::Property<GameObject*, GameObject*> Parent;
    const ObjectCollection& Children();
    void ToFront();
    void ToBack();
    Event<GameObject*> OrderChanged;
    int ChildIndex();
    
    template<class T>
    T* AddComponent();
    
    template<class T>
    T* AddComponent(GameObject* objectReference);
    
    template<class T>
    T* CloneComponent(GameObject* object);
    
    template<class T>
    T* GetComponent();
    
    template<class T>
    void EnableComponent(bool enable);
    
    template<class T>
    bool IsComponentEnabled();
    
    template<class T>
    void RemoveComponent();
    
    template<class T>
    bool IsComponentReference();
    
    void Remove();
    
    void AddComponent(int componentType);
    
    GameObject* Clone();
    GameObject* Clone(GameObject* parent);
    
    void Serialize(ISerializedProperty* serializedObject);
    void Deserialize(ISerializedProperty* serializedObject);
    
    GameWorld* World();
    
private:
    GameObject();
    ~GameObject();
    void ParentChanged(Pocket::Property<GameObject*, GameObject*>::EventData d);
    void Initialize(size_t numberOfComponents);
    void CountPointers();
    GameObject* CloneInternal(GameObject* parent);
    void UpdatePointers();
    
    ObjectCollection children;
    GameWorld* world;
    typedef void* Components;
    Components* components;
    typedef unsigned short SystemIndicies;
    SystemIndicies* systemIndicies;
    
    unsigned indexInList;
    
    ComponentMask activeComponents;
    ComponentMask enabledComponents;
    ComponentMask ownedComponents;
    
    friend class GameWorld;
    friend class IGameComponentType;
    template<class T>
    friend class GameComponentType;
    friend class ScriptWorld;
};

}