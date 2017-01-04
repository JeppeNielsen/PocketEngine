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
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void Update(float dt) override;
};