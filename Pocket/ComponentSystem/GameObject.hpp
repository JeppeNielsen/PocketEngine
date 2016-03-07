//
//  GameObject.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 12/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Property.hpp"
#include "IDHelper.hpp"
#include "TypeInfo.hpp"
#include "Container.hpp"
#include "GameConstants.hpp"

namespace Pocket {

class GameWorld;

#ifdef SCRIPTING_ENABLED

struct IGameObject {
    virtual void* GetComponent(int componentID) = 0;
    virtual void* AddComponent(int componentID) = 0;
    virtual void* AddComponent(int componentID, GameObject* referenceObject) = 0;
    virtual void RemoveComponent(int componentID) = 0;
    virtual void* GetScriptComponent(int componentID) = 0;
    virtual void* AddScriptComponent(int componentID) = 0;
    virtual void RemoveScriptComponent(int componentID) = 0;
};

#endif

class GameObject
#ifdef SCRIPTING_ENABLED
: public IGameObject
#endif
{
public:
    using ObjectCollection = std::vector<GameObject*>;

private:
    friend class GameWorld;
    
    using Bitset = GameConstants::Bitset;
    
    Bitset activeComponents;
    Bitset removedComponents;
    Bitset ownedComponents;
    
    using SystemIndices = int*;
    
    SystemIndices systemIndices;
    
    bool isRemoved;
    Container<GameObject>::ObjectInstance* instance;
    GameWorld* world;
    
    friend class Container<GameObject>::ObjectInstance;
    
    void Reset();
    void SetWorld(GameWorld* w);
    GameObject(const GameObject& other) = default;

protected:
    
    friend class IGameWorld;

    using ComponentPtr = void*;
    ComponentPtr* components;

    GameObject();
    virtual ~GameObject();
    
    ObjectCollection children;
public:

    Property<GameObject*> Parent;
    const ObjectCollection& Children() { return children; }
    Property<int> Order;
    
    template<typename T>
    T* GetComponent() {
        return (T*)components[IDHelper::GetComponentID<T>()];
    }
    
    void Remove();
    
    template<typename Component>
    bool HasComponent() const {
        return activeComponents[IDHelper::GetComponentID<Component>()];
    }
    
    template<typename Component>
    void RemoveComponent();
    
    template<typename Component>
    Component* AddComponent();
    
    template<typename Component>
    Component* AddComponent(GameObject* source);
    
 #if SCRIPTING_ENABLED
    void* GetComponent(int componentID) override;
    void* AddComponent(int componentID) override;
    void* AddComponent(int componentID, GameObject* referenceObject) override;
    void RemoveComponent(int componentID) override;
#else
    void* GetComponent(int componentID);
    void* AddComponent(int componentID);
    void* AddComponent(int componentID, GameObject* referenceObject);
    void RemoveComponent(int componentID);
#endif

    using SerializePredicate = std::function<bool(GameObject*, int)>;
    
    void ToJson(std::ostream& stream, SerializePredicate predicate = 0);
    
    void SetID(std::string id);
    std::string GetID();
    
private:
    template<typename Component>
    void SetComponent(typename Container<Component>::ObjectInstance* instance);
    
    void WriteJson(minijson::object_writer& writer, SerializePredicate predicate);
    void SerializeComponent(int componentID, minijson::array_writer& writer, bool isReference, std::string* referenceID );
    void AddComponent(minijson::istream_context& context, std::string componentName);
    
    template<typename Component>
    TypeInfo GetComponentTypeInfo() {
        Component* component = GetComponent<Component>();
        return component->GetType();
    }
    
#ifdef SCRIPTING_ENABLED
    //Scripting *******************************
    
    // Scripting Data
    using ScriptComponent = Container<void*>::ObjectInstance*;
    ScriptComponent* scriptComponents;
    
    int* scriptSystemIndices;
    
    GameConstants::ScriptBitset activeScriptComponents;
    GameConstants::ScriptBitset removedScriptComponents;

private:
    void ClearScriptingData();
    void InitializeScriptingData();
public:
    
    void* GetScriptComponent(int componentID) override;
    void* AddScriptComponent(int componentID) override;
    void RemoveScriptComponent(int componentID) override;
private:
    void CheckForScriptSystemsAddition(const std::vector<short>& systems, const GameConstants::Bitset& activeComponentsBefore, const typename GameConstants::ScriptBitset& activeScriptComponentsBefore);
    void CheckForScriptSystemsRemoval(const std::vector<short>& systems, const GameConstants::Bitset& activeComponentsBefore, const typename GameConstants::ScriptBitset& activeScriptComponentsBefore);
#endif
};

}