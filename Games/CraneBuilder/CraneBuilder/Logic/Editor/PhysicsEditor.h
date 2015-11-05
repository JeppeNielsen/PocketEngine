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
#include "Atlas.hpp"

using namespace Pocket;

class PhysicsEditor : public GameSystem {
public:
    PhysicsEditor();
    
    GameObject* atlas;
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    void Update(float dt);
    Property<PhysicsEditor*, InputManager*> Input;
    Property<PhysicsEditor*, bool> Enabled;
private:
    void ParticleClicked(TouchData d, GameObject* object);
    void SpringClicked(TouchData d, GameObject* object);
    void BackgroundClicked(TouchData d, GameObject* object);
    
    GameObject* CreateParticle();
    Spring* CreateSpring();
    
    GameObject* background;
    
    enum class State {
        Idle,
        ParticleClicked,
        SpringClicked,
    };
    
    State currentState;
};