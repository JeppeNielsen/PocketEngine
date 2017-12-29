#pragma once
#include "GameSystem.hpp"
#include "Vector3.hpp"
#include "Transform.hpp"

using namespace Pocket;

struct TransformFollower { 
   GameObjectHandle target;
   Vector3 position;
   Vector3 lookAt;
   float smoothness;

   TransformFollower() {
       smoothness = 0.95f;
       position = {0,10,10};
   }
};

struct TransformFollowerSystem : GameSystem<TransformFollower, Transform> {
    void Initialize() override;
   void ObjectAdded(GameObject* object) override;
   void ObjectRemoved(GameObject* object) override;
   void Update(float dt) override;
};

