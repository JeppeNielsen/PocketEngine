#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Velocity.cpp"
#include "Collidable.cpp"
#include <iostream>

using namespace Pocket;

struct VelocityCollider : public GameSystem<Velocity, Collidable> {

    void ObjectAdded(GameObject* object) {
        object->GetComponent<Collidable>()->OnCollision.Bind(this, &VelocityCollider::OnCollision, object);
    }

    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Collidable>()->OnCollision.Unbind(this, &VelocityCollider::OnCollision, object);
    }

    void OnCollision(CollisionData e, GameObject* object) {
        std::cout << "COLLISION"<<std::endl;
        GameObject* other = e.a == object ? e.b : e.a;
        Transform* otherTransform = other->GetComponent<Transform>();
        if (!otherTransform) return;

        Velocity* vel = object->GetComponent<Velocity>();
        vel->velocity = -vel->velocity;
    }
};