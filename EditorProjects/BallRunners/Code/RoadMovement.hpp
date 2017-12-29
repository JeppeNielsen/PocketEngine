#pragma once
#include "GameSystem.hpp"
#include "RoadPosition.hpp"

using namespace Pocket;

struct RoadMovement { 
   float speed;
};

struct RoadMovementSystem : GameSystem<RoadMovement, RoadPosition> {
   void ObjectAdded(GameObject* object) override;
   void ObjectRemoved(GameObject* object) override;
   void Update(float dt) override;
};

