

#include "TransformController.hpp"

void TransformControllerSystem::Update(float dt) {
    for(auto o : Objects()) {
        InputController* input = o->GetComponent<InputController>();
        if (input->IsButtonDown("down")) {
            TransformController* controller = o->GetComponent<TransformController>();
            Transform* t = o->GetComponent<Transform>();
            t->Position += {0,-controller->speed*dt, 0};
        } else if (input->IsButtonDown("up")) {
            TransformController* controller = o->GetComponent<TransformController>();
            Transform* t = o->GetComponent<Transform>();
            t->Position += {0,controller->speed*dt, 0};
        }
    }
}