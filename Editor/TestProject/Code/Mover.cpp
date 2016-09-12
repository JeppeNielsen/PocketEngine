#include "GameSystem.hpp"
#include "Transform.hpp"

struct Mover {
    Vector3 speed;
};

using namespace Pocket;

struct MoverSystem : public GameSystem<Transform, Mover> {
    void Update(float dt) {
        for(auto o : Objects()) {
            auto transform = o->GetComponent<Transform>();
            auto mover = o->GetComponent<Mover>();
            
            transform->Position += mover->speed * dt;


        }
    }
};