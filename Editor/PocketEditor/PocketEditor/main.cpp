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
    struct RotatorSystem : public GameSystem<Transform, Rotator> {
        void Update(float dt) {
            for(auto o : Objects()) {
                o->GetComponent<Transform>()->EulerRotation += o->GetComponent<Rotator>()->speed * dt;
            }
        }
    };
    
    void Initialize() {
        
        world.CreateSystem<RenderSystem>();
        world.CreateSystem<RotatorSystem>();
        
        camera = world.CreateObject();
        camera->AddComponent<Camera>()->Viewport = Context().Viewport();
        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 40;
        
        for (int x=0; x<20; x++) {
            for(int y=0; y<20; y++) {
                CreateCube({(float)x,(float)y,0});
            }
        }
    }
    
    void CreateCube(Vector3 pos) {
        cube = world.CreateObject();
        cube->AddComponent<Transform>()->Position = pos;
        cube->AddComponent<Rotator>()->speed = { 2,1,0 };
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube->AddComponent<Material>();
        
        auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
        }
    }
    
    void Update(float dt) {
        camera->GetComponent<Camera>()->Viewport = Context().Viewport();
        world.Update(dt);
    }
    
    void Render() {
        glClearColor(1, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<Game>();
	return 0;
}