//
//  GameComponent.h
//  GameComponentSystem
//
//  Created by Jeppe Nielsen on 9/24/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameComponentType.hpp"
#include "TypeDefs.hpp"
#include "IPointable.hpp"
#include "Serializable.hpp"

namespace Pocket {

class GameObject;

template<class T>
class GameComponent : public IPointable<T>, public ISerializable {
public:
    virtual ~GameComponent() { }
    static int ID;
    
    virtual std::string Name() = 0;
    virtual void Reset();
    
private:
    
    int referenceCount;
    int indexInList;

    template<class U>
    friend class GameComponentType;
    
    friend class GameObject;
};

}

template<class T>
int Pocket::GameComponent<T>::ID = GameComponentTypeFactory::GetID<T>();

template<class T>
void Pocket::GameComponent<T>::Reset() {}

#define Component(Class) \
class Class : public GameComponent<Class> { \
public: \
    virtual std::string Name() { return #Class; } \
    static std::string NameStatic() { return #Class; } \
public: \



