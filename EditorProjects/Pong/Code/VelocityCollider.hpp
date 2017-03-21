#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Velocity.hpp"
#include "RectCollider.hpp"
#include <iostream>

using namespace Pocket;

struct VelocityCollider : public GameSystem<Velocity, RectCollider> {
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void OnCollision(GameObject* collisionObject, GameObject* object);
};