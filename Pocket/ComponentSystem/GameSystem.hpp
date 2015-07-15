//
//  GameSystem.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 9/29/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "TypeDefs.hpp"
#include <map>

namespace Pocket {
  
class GameWorld;
class IGameComponentType;

class GameSystem {
public:
    virtual ~GameSystem();
    const ObjectCollection& Objects();
protected:
    
    virtual void Initialize();
    
    virtual void ObjectAdded(GameObject* object);
    virtual void ObjectRemoved(GameObject* object);
    virtual void Update(float dt);
    virtual void Render();
    virtual void AddedToWorld(GameWorld& world);
    
    void SetMetaData(GameObject* object, void* data);
    void* GetMetaData(GameObject* object);
    
public:
    template<class T>
    void AddComponent();
protected:
    GameWorld* World();
    
private:
    
    GameWorld* world;
    unsigned indexInList;
    ComponentMask aspect;
    ObjectCollection objects;
    
    friend class GameWorld;
    friend class IGameComponentType;
    
    typedef std::map<GameObject*, void*> MetaData;
    
    MetaData metaData;
};

}

#define _SYSTEM_NODE1(C1) \
void Initialize() { \
AddComponent<C1>(); \
} \

#define _SYSTEM_NODE2(C1, C2) \
void Initialize() { \
AddComponent<C1>(); \
AddComponent<C2>(); \
} \

#define _SYSTEM_NODE3(C1, C2, C3) \
void Initialize() { \
AddComponent<C1>(); \
AddComponent<C2>(); \
AddComponent<C3>(); \
} \

#define _SYSTEM_NODE4(C1, C2, C3, C4) \
void Initialize() { \
AddComponent<C1>(); \
AddComponent<C2>(); \
AddComponent<C3>(); \
AddComponent<C4>(); \
} \

#define _SYSTEM_NODE5(C1, C2, C3, C4, C5) \
void Initialize() { \
AddComponent<C1>(); \
AddComponent<C2>(); \
AddComponent<C3>(); \
AddComponent<C4>(); \
AddComponent<C5>(); \
} \

#define _SYSTEM_START(NAME) \
class NAME : public GameSystem { \
friend class GameWorld; \
public: \

#define _SYSTEM_1(NAME, C1) \
_SYSTEM_START(NAME) \
_SYSTEM_NODE1(C1) \

#define _SYSTEM_2(NAME, C1, C2) \
_SYSTEM_START(NAME) \
_SYSTEM_NODE2(C1,C2) \

#define _SYSTEM_3(NAME, C1, C2, C3) \
_SYSTEM_START(NAME) \
_SYSTEM_NODE3(C1,C2,C3) \

#define _SYSTEM_4(NAME, C1, C2, C3, C4) \
_SYSTEM_START(NAME) \
_SYSTEM_NODE4(C1,C2,C3,C4) \

#define _SYSTEM_5(NAME, C1, C2, C3, C4, C5) \
_SYSTEM_START(NAME) \
_SYSTEM_NODE5(C1,C2,C3,C4,C5) \

#define GET_MACRO(_1,_2,_3,_4,_5, _6, NAME,...) NAME
#define SYSTEM(...) GET_MACRO(__VA_ARGS__, _SYSTEM_5, _SYSTEM_4, _SYSTEM_3, _SYSTEM_2, _SYSTEM_1)(__VA_ARGS__)
//#define GET_MACRO(_1,_2,_3, NAME,...) NAME
//#define SYSTEM(...) GET_MACRO(__VA_ARGS__, _SYSTEM_2, _SYSTEM_1)(__VA_ARGS__)






