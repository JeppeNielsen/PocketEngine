#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "TouchSystem.hpp"
#include "DraggableSystem.hpp"

struct GameWorldSettings :
    GameSettings<
        RenderSystem<GameWorldSettings>,
        TouchSystem<GameWorldSettings>,
        DraggableSystem<GameWorldSettings>
    >
{};


using namespace Pocket;

class Game : public GameState<Game> {
public:
    GameWorld<GameWorldSettings> world;
    
    using GameObject = GameObject<GameWorldSettings>;
    
    GameObject* camera;
    GameObject* cube;
    
    void Initialize() {
    
        world.Initialize();
        
        auto& renderSystem = world.GetSystem<RenderSystem<GameWorldSettings>>();
        renderSystem.DefaultShader = &renderSystem.Shaders.LitColored;
        
        world.GetSystem<TouchSystem<GameWorldSettings>>().Input = &Input;
        
        camera = world.CreateObject();
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        camera->AddComponent<Transform>()->Position = { 0, 0, 30 };
        camera->GetComponent<Camera>()->FieldOfView = 40;
        
        cube = world.CreateObject();
        cube->AddComponent<Transform>()->Position = {0,0,0};
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
        }
        cube->AddComponent<Material>();
        cube->AddComponent<Touchable>()->Click += event_handler(this, &Game::Down, cube);
        cube->AddComponent<Draggable>()->Movement = Draggable::MovementMode::XYPlane;
        
    }
    
    void Down(TouchData e, GameObject* o) {
        //o->GetComponent<Transform>()->Position += {0.2f,0,0};
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