#include "GameSystem.hpp"
#include "Mesh.hpp"

struct Blinker {
    float speed;
    float time;
};

using namespace Pocket;

struct BlinkerSystem : public GameSystem<Blinker, Mesh> {
    void Update(float dt) {
        for(auto o : Objects()) {
            auto blinker = o->GetComponent<Blinker>();
            blinker->time += blinker->speed * dt;
            auto& verts = o->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
            for(auto& v : verts) {
                float color = 0.5f + sinf(blinker->time) * 0.5f;
                v.Color = Colour(color,color,1.0f-color,1.0f);
            }
        }
    }
};