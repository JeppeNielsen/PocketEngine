#pragma once
#include "GameSystem.hpp"
#include "Road.hpp"

using namespace Pocket;

struct RoadGenerator { 
   float frequency;
   float time;

   RoadGenerator() {
       frequency = 1.0f;
       time = 0;
   }
};

struct RoadGeneratorSystem : GameSystem<RoadGenerator, Road> {
   void ObjectAdded(GameObject* object) override;
   void ObjectRemoved(GameObject* object) override;
   void Update(float dt) override;
};

