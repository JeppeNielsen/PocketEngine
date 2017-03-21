#pragma once
#include "Vector3.hpp"
#include "Transform.hpp"
#include "GameSystem.hpp"

using namespace Pocket;

struct Rotator {
    Vector3 speed;
};

class RotatorSystem : public GameSystem<Rotator, Transform> {
public:
    void Update(float dt);
};

