#include "GameSystem.hpp"
#include <string>
#include "Transform.hpp"
#include "InputController.hpp"
#include <iostream>

using namespace Pocket;

struct Controller {
    std::string upKey;
    std::string downKey;
    float speed;
};

struct ControllerSystem : public GameSystem<Controller, Transform, InputController> {
    void ObjectAdded(GameObject* object) override {
        std::cout << "Added"<<std::endl;
    }

    void ObjectRemoved(GameObject* object) override {
        std::cout << "Removed"<<std::endl;
    }

    void Update(float dt) override {
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
};