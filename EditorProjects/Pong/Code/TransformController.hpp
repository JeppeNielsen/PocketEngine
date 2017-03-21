
#include "GameSystem.hpp"
#include "Transform.hpp"
#include "InputController.hpp"

using namespace Pocket;

struct TransformController {
    float speed;
};

struct TransformControllerSystem : public GameSystem<Transform, TransformController, InputController> {
    void Update(float dt) override;
};