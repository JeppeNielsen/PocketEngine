//
//  PieceControllerSystem.hpp
//  Tetris
//
//  Created by Jeppe Nielsen on 4/5/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "InputManager.hpp"
#include "PieceController.hpp"
#include "PieceCollider.hpp"
#include <set>

using namespace Nano;

class PieceControllerSystem : public GameSystem {
public:
    
    void SetInputManager(InputManager& input);
    
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    
private:
    void ButtonDown(std::string button);
    
    void ModifyTransform(GameObject* colliderObject, std::string button);
    
    typedef std::set<std::string> Buttons;
    Buttons buttonDowns;
    
};