#include "Rotatable.hpp"

void RotatableSystem::ObjectAdded(GameObject* object) {

}

void RotatableSystem::ObjectRemoved(GameObject* object) {

}

void RotatableSystem::Update(float dt) {
    for(auto o : Objects()) {
        o->GetComponent<Transform>()->EulerRotation += o->GetComponent<Rotatable>()->angularVelocity * dt;
    }
}
