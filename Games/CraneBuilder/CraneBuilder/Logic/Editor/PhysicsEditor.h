//
//  BuilderEditor.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/19/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "GameWorld.hpp"
#include "Particle.h"
#include "Touchable.hpp"
#include "InputManager.hpp"
#include "Spring.h"

using namespace Pocket;

class PhysicsEditor : public GameSystem {
public:

    void Initialize();

    void ObjectAdded(GameObject* object);

    void ObjectRemoved(GameObject* object);
    
    void Update(float dt);

    void SetInputManager(InputManager& input);

private:
    void ParticleClicked(TouchData d, GameObject* object);
    void TouchUp(TouchEvent e);
    
    GameObject* modifingParticle;
    GameObject* createdParticle;
    GameObject* cameraObject;
    
    InputManager* input;
    
    GameObject* CreateParticle();
    Spring* CreateSpring();
    
    bool touchUp;
    bool particleClicked;

};