
#include "Rotator.hpp"
#include "Box.hpp"

void RotatorSystem::Update(float dt) {
    for(auto o : Objects()) {
        o->GetComponent<Transform>()->EulerRotation += o->GetComponent<Rotator>()->speed * dt;
    }
}
