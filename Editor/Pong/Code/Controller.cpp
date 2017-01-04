#include "Controller.hpp"

using namespace Pocket;

void ControllerSystem::ObjectAdded(GameObject* object) {
    std::cout << "Added"<<std::endl;
}

void ControllerSystem::ObjectRemoved(GameObject* object) {
    std::cout << "Removed"<<std::endl;
}

void ControllerSystem::Update(float dt) {
    for(auto o : Objects()) {
        auto input = o->GetComponent<InputController>();
        auto controller = o->GetComponent<Controller>();

        Vector3 velocity = 0;

        if (input->IsButtonDown(controller->upKey)) {
            velocity.y += controller->speed;
        }

        if (input->IsButtonDown(controller->downKey)) {
            velocity.y -= controller->speed;
        }

        o->GetComponent<Transform>()->Position += velocity * dt;
    }
}
