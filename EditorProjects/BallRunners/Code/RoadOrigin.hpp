#pragma once
#include "GameSystem.hpp"
#include "Road.hpp"
#include "RoadPosition.hpp"

using namespace Pocket;

struct RoadOrigin { 
   float variable;
};

struct RoadOriginSystem : GameSystem<RoadOrigin, RoadPosition, Road> {
   void ObjectAdded(GameObject* object) override;
   void ObjectRemoved(GameObject* object) override;
   void Update(float dt) override;
};

