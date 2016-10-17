#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Vector3.hpp"

using namespace Pocket;

struct Velocity {
    Vector3 velocity;
};

struct VelocitySystem : public GameSystem<Velocity, Transform> {
    void Update(float dt) override {
        for(auto o : Objects()) {
            o->GetComponent<Transform>()->Position += o->GetComponent<Velocity>()->velocity * dt;
        }
    }
};
