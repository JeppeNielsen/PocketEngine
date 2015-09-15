//
//  World.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 9/24/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameComponent.hpp"
#include "GameObject.hpp"
#include "GameSystem.hpp"
#include "GameFactory.hpp"
#include <vector>
#include <memory>
#include <string>
#include <istream>
#if EMSCRIPTEN
#if ENABLE_SCRIPTING
    #undef ENABLE_SCRIPTING
#endif
#endif

namespace minijson {
    class object_writer;
};

namespace Pocket {

class GameWorld {
public:
    
    friend class GameObject;
    friend class ScriptWorld;
    
    GameWorld();
    ~GameWorld();
    
    GameObject* CreateObject();
    GameObject* CreateObjectFromJson(std::istream& jsonStream);
    
    void Update(float dt);
    void Render();
    
    void DeleteAllObjects();
    
    const ObjectCollection& Objects();
    const ObjectCollection& Children();
    
    template<class T>
    T* CreateSystem();
    
    template<class T>
    T* CreateOrGetSystem();
    
    template<typename T>
    T* GetSystem();
    
    template<typename T>
    T* CreateFactory();
    
    typedef std::vector<IGameComponentType*> ComponentTypes;
    const ComponentTypes& ComponentTypesList();
    
#ifdef ENABLE_SCRIPTING
    void InitializeScripts();
    bool Build(std::vector<std::string> scriptFilenames);
    bool CallMain();
    
    ScriptWorld* GetScriptWorld();
#endif
    
private:
    
    
    void UpdateRemovedObjects();
    void UpdateRemovedObject(GameObject* object);
    void UpdateChangedComponents();
    void UpdateRemovedComponents();
    
    void* AddComponent(GameObject* object, int componentID);
    void* AddComponent(GameObject* object, int componentID, GameObject* referenceObject);
    void RemoveComponent(GameObject* object, int componentID);
    void EnableComponent(GameObject* object, int componentID, bool enable);
    void* CloneComponent(GameObject* object, int componentID, GameObject* source);
    void UpdatePointers(GameObject* object, int componentID);
    
    void AddSystem(GameSystem* system, int componentID);
    
    void WriteJsonComponent(minijson::array_writer& writer, GameObject* object, int componentID);
    void ReadJsonComponent(minijson::istream_context& context, GameObject* object, int componentID);
    GameObject* CreateGameObjectJson(minijson::istream_context& context);
    void CreateObjectID(GameObject* object, const std::string& id);
    GameObject* FindObjectFromID(const std::string& id);
    std::string* FindIDFromObject(GameObject* object);
    std::string* FindIDFromReferenceObject(GameObject* referenceObject, int componentID);
    GameObject* FindFirstObjectWithComponentID(int componentID);
   
    ComponentTypes componentTypes;
    ComponentTypes changedComponentTypes;
    ComponentTypes removedComponentTypes;
    
    ObjectCollection activeObjects;
    ObjectCollection freeObjects;
    typedef std::vector<GameObject*> RemovedObjects;
    RemovedObjects removedObjects;
    
    typedef std::vector<GameSystem*> Systems;
    Systems systems;
    friend class GameSystem;
    
    ScriptWorld* scriptWorld;
    ObjectCollection children;
    typedef std::map<void*, void*> PointerMap;
    PointerMap pointerMap;
    typedef std::map<void*, int> PointerCounter;
    PointerCounter pointerCounter;
    
    struct ObjectID {
        GameObject* object;
        std::string id;
    };
    
    typedef std::vector<ObjectID> ObjectIDs;
    ObjectIDs objectIDs;
    
};

}

//--------------------------- <GameWorld> -----------------------------

template<class T>
T* Pocket::GameWorld::CreateSystem() {
    T* system = new T();
    system->indexInList = (unsigned)systems.size();
    systems.push_back(system);
    system->world = this;
    system->aspect = 0;
    system->Initialize();
    system->AddedToWorld(*this);
    return system;
}

template<class T>
T* Pocket::GameWorld::CreateOrGetSystem() {
    T* system = GetSystem<T>();
    if (!system) {
        system = CreateSystem<T>();
    }
    return system;
}

template<typename T>
T* Pocket::GameWorld::GetSystem() {
    for (size_t i = 0; i<systems.size(); i++) {
        T* system = dynamic_cast<T*>(systems[i]);
        if (system) return system;
    }
    return 0;
}

template<typename T>
T* Pocket::GameWorld::CreateFactory() {
    T* factory = new T();
    factory->world = this;
    factory->Initialize();
    return factory;
}

//--------------------------- </GameWorld> -----------------------------


//--------------------------- <GameObject> -----------------------------

template<class T>
T* Pocket::GameObject::AddComponent() {
    return (T*)world->AddComponent(this, T::ID);
}

template<class T>
T* Pocket::GameObject::AddComponent(GameObject* objectReference) {
    return (T*)world->AddComponent(this, T::ID, objectReference);
}

template<class T>
T* Pocket::GameObject::GetComponent() {
    return (T*)components[T::ID];
}

template<class T>
void Pocket::GameObject::RemoveComponent() {
    world->RemoveComponent(this, T::ID);
}

template<class T>
bool Pocket::GameObject::IsComponentEnabled() {
    ComponentMask mask = world->componentTypes[T::ID]->mask;
    return (enabledComponents & mask) == mask;
}

template<class T>
void Pocket::GameObject::EnableComponent(bool enable) {
    world->EnableComponent(this, T::ID, enable);
}

template<class T>
T* Pocket::GameObject::CloneComponent(GameObject* source) {
    return (T*)world->CloneComponent(this, T::ID, source);
}

template<class T>
bool Pocket::GameObject::IsComponentReference() {
    return IsComponentReference(T::ID);
}

//--------------------------- </GameObject> -----------------------------


//--------------------------- <GameSystem> -----------------------------

template<class T>
void Pocket::GameSystem::AddComponent() {
   world->AddSystem(this, T::ID);
}

//--------------------------- </GameSystem> -----------------------------

//--------------------------- <GameFactory> -----------------------------

template<typename T>
T* Pocket::GameFactory::CreateSystem() {
    T* system = World()->GetSystem<T>();
    if (system) return system;
    return World()->CreateSystem<T>();
}

//--------------------------- </GameFactory> -----------------------------


