
#pragma once
#include <iostream>
#include <vector>
#include "GameSystem.hpp"
#include "Property.hpp"
#include "Transform.hpp"

using namespace Pocket;

struct Turner {
    Vector3 speed;
    bool reverse;
};

struct TurnerSystem : public GameSystem<Transform, Turner> {
    void Update(float dt) {
        for(auto o : Objects()) {
            o->GetComponent<Transform>()->EulerRotation += o->GetComponent<Turner>()->speed * (o->GetComponent<Turner>()->reverse ?  -dt : dt);
        }
    }
};

