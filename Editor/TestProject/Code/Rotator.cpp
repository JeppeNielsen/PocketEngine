
#pragma once
#include <iostream>
#include <vector>
#include "GameSystem.hpp"
#include "Property.hpp"
#include "Transform.hpp"

using namespace Pocket;

struct Rotator {
    Vector3 speed;
};

struct RotatorSystem : public GameSystem<Transform, Rotator> {
    void Update(float dt) {
        for(auto o : Objects()) {
            o->GetComponent<Transform>()->EulerRotation += o->GetComponent<Rotator>()->speed * dt;
        }
    }
};

