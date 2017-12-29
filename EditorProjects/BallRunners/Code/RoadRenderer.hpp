#pragma once
#include "GameSystem.hpp"
#include "Road.hpp"
#include "Mesh.hpp"
#include "RoadPosition.hpp"

using namespace Pocket;

struct RoadRenderer { 
    float Depth;
    int segments;
    float vScale;
    float renderOffset;
    float origin;
};

struct RoadRendererSystem : GameSystem<RoadRenderer, Road, Mesh, RoadPosition> {
   void Initialize() override;
   void ObjectAdded(GameObject* object) override;
   void ObjectRemoved(GameObject* object) override;
   void Update(float dt) override;
   void PositionChanged(GameObject* object);
};

