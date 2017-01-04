#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Velocity.hpp"
#include "Collidable.hpp"
#include <iostream>

using namespace Pocket;

struct VelocityCollider : public GameSystem<Velocity, Collidable> {
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void OnCollision(GameObject* collisionObject, GameObject* object);
};