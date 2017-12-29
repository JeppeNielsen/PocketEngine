#pragma once
#include "GameSystem.hpp"
#include "Vector3.hpp"
#include "Transform.hpp"

using namespace Pocket;

struct Rotatable { 
   Vector3 angularVelocity;
};

struct RotatableSystem : GameSystem<Rotatable, Transform> {
   void ObjectAdded(GameObject* object) override;
   void ObjectRemoved(GameObject* object) override;
   void Update(float dt) override;
};

