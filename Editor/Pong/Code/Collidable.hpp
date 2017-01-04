#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Vector3.hpp"
#include "Event.hpp"
#include <iostream>

using namespace Pocket;

struct Collidable {
    float radius;
    
    std::vector<GameObject*> collisions;
    std::vector<GameObject*> previousCollisions;
    
    Event<GameObject*> Enter;
    Event<GameObject*> Exit;
};

struct CollisionSystem : public GameSystem<Transform, Collidable> {
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void Update(float dt) override;
};
