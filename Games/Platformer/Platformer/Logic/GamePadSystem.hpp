//
//  GamePadSystem.h
//  Platformer
//
//  Created by Jeppe Nielsen on 27/04/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "GamePadController.hpp"
#include "CharacterController.hpp"
#include "InputManager.hpp"
#include <map>
#include "Groundable.hpp"

using namespace Nano;

SYSTEM(GamePadSystem, GamePadController, CharacterController, Groundable)
    public:
        void SetInput(InputManager* input);
        void ObjectAdded(GameObject* object);
        void ObjectRemoved(GameObject* object);

        void Update(float dt);

    private:
        void ButtonDown(GamePadData d);
        void ButtonUp(GamePadData d);
        void AnalogChanged(GamePadData d);

        GameObject* FindObject(int index);

        typedef std::map<int, Vector2> Movement;
        Movement movement;

    private:
        InputManager* input;
};