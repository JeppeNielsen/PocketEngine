#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"

using namespace Pocket;

class Game : public GameState<Game> {
public:
    GameWorld world;
    RenderSystem* renderer;
    GameObject* camera;
    GameObject* cube;
    
    void Initialize() {
        
        renderer = world.CreateSystem<RenderSystem>();
        
        camera = world.CreateObject();
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 40;
        
        cube = world.CreateObject();
        cube->AddComponent<Transform>();
        cube->AddComponent<Mesh>()->AddCube(0, 1);
        cube->AddComponent<Material>();
        
        Mesh::VerticesList& verts = cube->GetComponent<Mesh>()->Vertices();
        
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
        }
        
        Input.GamePad.AnalogChanged += event_handler(this, &Game::AnalogChanged);
    }
    
    void AnalogChanged(GamePadData data) {
        Quaternion rot = Quaternion(data.Direction.x * 0.05f, {0,1,0}) * Quaternion(data.Direction.y * 0.05f, {1,0,0});
        cube->GetComponent<Transform>()->Rotation *= rot;
    }
    
    void Update(float dt) {
        world.Update(dt);
    }
    
    void Render() {
        renderer->Render();
    }
};

int main() {
    Engine e;
    e.Start<Game>();
	return 0;
}