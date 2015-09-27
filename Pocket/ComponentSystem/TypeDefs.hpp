//
//  TypeDefs.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 9/25/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <bitset>

namespace Pocket {
    class GameObject;
    class SerializedComponent;
    class ISerializable;
}


//typedef uint64_t ComponentMask;
typedef std::bitset<128> ComponentMask;
typedef std::vector<Pocket::GameObject*> ObjectCollection;
typedef std::vector<Pocket::ISerializable*> SerializableCollection;


