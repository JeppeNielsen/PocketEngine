//
//  CreatorSystem.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 02/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Creator.h"
#include "Particle.h"
#include "Transform.hpp"
#include "Touchable.hpp"
#include "InputManager.hpp"
#include "Spring.h"

using namespace Pocket;

struct CreatorSystem : public GameSystem<Creator, Particle, Transform, Touchable> {

    void Initialize();
    void ObjectAdded(GameObject* object);
    void ObjectRemoved(GameObject* object);

    void Update(float dt);

    std::function<GameObject*()> CreateParticle;
    std::function<GameObject*()> CreateSpring;
    std::function<bool()> Continuous;

    void Cancel();

    Vector2 gridSize;

private:

    Vector3 Grid(Vector3 position);

    struct Particles: public GameSystem<Particle> {
        GameObject* FindNearestParticle(const Vector2& position, GameObject* excludeThis, float& minDistance);
    };

    struct Springs : public GameSystem<Spring> {
        bool DoesSpringExists(Particle* p1, Particle* p2);
    };

    Particles* particles;
    Springs* springs;

    void ParticleClicked(TouchData d, GameObject* particleObject);

    void TouchDown(TouchEvent e);

    void StartFromParticle(GameObject* particleObject);

    GameObject* sourceParticle;
    GameObject* createdParticle;
    GameObject* createdSpring;
    TouchData currentTouch;

    enum class State {
        Idle,
        Creating,
        Succes,
        Cancel,
    };

    State currentState;
};
