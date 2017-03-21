#pragma once
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Vector3.hpp"
#include "Event.hpp"
#include "Sizeable.hpp"
#include "Box.hpp"
#include <iostream>

using namespace Pocket;

struct RectCollider {
    std::vector<GameObject*> collisions;
    std::vector<GameObject*> previousCollisions;
    
    Event<GameObject*> Enter;
    Event<GameObject*> Exit;
};

struct RectCollisionSystem : public GameSystem<Transform, RectCollider, Sizeable> {
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void Update(float dt) override;
private:
    Box GetCollisionBox(GameObject* object);
};
