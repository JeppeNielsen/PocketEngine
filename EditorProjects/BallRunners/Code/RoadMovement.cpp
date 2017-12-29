#include "RoadMovement.hpp"

void RoadMovementSystem::ObjectAdded(GameObject* object) {

}

void RoadMovementSystem::ObjectRemoved(GameObject* object) {

}

void RoadMovementSystem::Update(float dt) {
    for(auto o : Objects()) {
        o->GetComponent<RoadPosition>()->Position += o->GetComponent<RoadMovement>()->speed * dt;
    }
}
