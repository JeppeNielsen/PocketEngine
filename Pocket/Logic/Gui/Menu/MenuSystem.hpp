//
//  MenuSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Menu.hpp"
#include "MenuAnimator.hpp"
#include "TransformAnimator.hpp"
#include "TransformAnimationDatabase.hpp"

using namespace Pocket;

class MenuSystem : public GameSystem {
public:
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
private:
    void ActiveMenuChanged(Property<Menu*, std::string>::EventData d, GameObject* object);
    
};