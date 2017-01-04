//
//  MenuButtonSystem.h
//  GirlsNightOut
//
//  Created by Jeppe Nielsen on 8/27/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameSystem.hpp"
#include "MenuButton.hpp"
#include "Touchable.hpp"

using namespace Pocket;

class MenuButtonSystem : public GameSystem<MenuButton, Touchable> {
public:
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
private:
    
    void ButtonClick(TouchData d, GameObject* object);
    
};