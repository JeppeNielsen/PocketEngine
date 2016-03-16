#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"

using namespace Pocket;

class Game : public GameState<Game> {
public:
    GameWorld world;
    GameObject* camera;
    GameObject* cube;
    
    struct Rotator { Vector3 speed; };
    struct RotatorSystem : public GameSystem<Transform, Rotator>() {
        void Update(float dt) {
            for(auto o : Objects) { o->GetComponent<Transform>()->Rotation += o->GetComponent<Rotator>()->speed * dt; }
        }
    };
    
    void Initialize() {
        
        camera = world.CreateObject();
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 40;
        
        cube = world.CreateObject();
        cube->AddComponent<Transform>();
        cube->AddComponent<Rotator>()->speed = { 2,1,0 };
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube->AddComponent<Material>();
        
        auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
        }
    }
    
    void Update(float dt) {
        world.Update(dt);
    }
    
    void Render() {
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<Game>();
	return 0;
}