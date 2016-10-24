//
//  MenuSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameSystem.hpp"
#include "Menu.hpp"
#include "MenuAnimator.hpp"
#include "TransformAnimator.hpp"
#include "TransformAnimationDatabase.hpp"

namespace Pocket {
class MenuSystem : public GameSystem<Menu, MenuAnimator, TransformAnimator, TransformAnimationDatabase> {
public:
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
private:
    void ActiveMenuChanged(GameObject* object);
    
};

}