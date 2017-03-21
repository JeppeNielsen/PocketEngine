#include "VelocityCollider.hpp"

using namespace Pocket;

void VelocityCollider::ObjectAdded(GameObject* object) {
    object->GetComponent<RectCollider>()->Enter.Bind(this, &VelocityCollider::OnCollision, object);
}

void VelocityCollider::ObjectRemoved(GameObject* object) {
    object->GetComponent<RectCollider>()->Enter.Unbind(this, &VelocityCollider::OnCollision, object);
}

void VelocityCollider::OnCollision(GameObject* collisionObject, GameObject* object) {
    std::cout << "COLLISION"<<std::endl;
    Transform* otherTransform = collisionObject->GetComponent<Transform>();
    if (!otherTransform) return;

    Velocity* vel = object->GetComponent<Velocity>();
    vel->velocity = -vel->velocity;
}
