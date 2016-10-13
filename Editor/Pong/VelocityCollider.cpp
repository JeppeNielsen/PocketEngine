#include "GameSystem.hpp"
#include "Transform.hpp"
#include "Velocity.cpp"
#include "Collidable.cpp"
#include <iostream>

using namespace Pocket;

struct VelocityCollider : public GameSystem<Velocity, Collidable> {

    void ObjectAdded(GameObject* object) {
        object->GetComponent<Collidable>()->Enter.Bind(this, &VelocityCollider::OnCollision, object);
    }

    void ObjectRemoved(GameObject* object) {
        object->GetComponent<Collidable>()->Enter.Unbind(this, &VelocityCollider::OnCollision, object);
    }

    void OnCollision(GameObject* collisionObject, GameObject* object) {
        std::cout << "COLLISION"<<std::endl;
        Transform* otherTransform = collisionObject->GetComponent<Transform>();
        if (!otherTransform) return;

        Velocity* vel = object->GetComponent<Velocity>();
        vel->velocity = -vel->velocity;
    }
};