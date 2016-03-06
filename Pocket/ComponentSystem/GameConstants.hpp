//
//  GameConstants.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 05/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <bitset>
#include <vector>
#include <functional>
#include <string>

class GameObject;
class IGameSystem;
class IContainer;

struct GameConstants {
    using Bitset = std::bitset<64>;
    
    using Systems = std::vector<IGameSystem*>;
    using SystemBitsets = std::vector<Bitset>;
    
    using Components = std::vector<IContainer*>;
    using ComponentNames = std::vector<std::string>;
    using ComponentSystems = std::vector<std::vector<IGameSystem*>>;
    using Actions = std::vector<std::function<void()>>;
#if SCRIPTING_ENABLED
    using ScriptBitset = std::vector<bool>;
#endif
};