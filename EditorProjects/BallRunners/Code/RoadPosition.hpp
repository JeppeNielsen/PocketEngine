#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Road.hpp"

using namespace Pocket;

struct RoadPosition { 
   Property<float> Position;
};

struct RoadPositionSystem : GameSystem<RoadPosition, Road, Transform> {
   void ObjectAdded(GameObject* object) override;
   void ObjectRemoved(GameObject* object) override;
   void Update(float dt) override;
   void PositionChanged(GameObject* object);
};