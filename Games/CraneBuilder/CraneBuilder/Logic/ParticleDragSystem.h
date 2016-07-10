//
//  ParticleDragSystem.h
//  CraneCreator
//
//  Created by Jeppe Nielsen on 12/4/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "Particle.h"
#include "Touchable.hpp"

using namespace Pocket;

class ParticleDragSystem : public GameSystem<> {
public:
    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);
    
private:
    void Down(TouchData data);
    void Up(TouchData data);
};