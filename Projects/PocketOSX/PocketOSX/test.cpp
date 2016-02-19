#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "TouchSystem.hpp"
#include "DraggableSystem.hpp"
#include "TransformHierarchy.hpp"

struct Rotator {
    Vector3 angles;
};

template<typename T>
struct RotatorSystem : GameSystem<T, Transform, Rotator> {
    void ObjectAdded(GameObject<T>* o) {}
    void ObjectRemoved(GameObject<T>* o) {}
    void Update(float dt) {
        for(auto o : this->Objects()) {
            auto* t = o->template GetComponent<Transform>();
            auto* r = o->template GetComponent<Rotator>();
            t->Rotation *= Quaternion(r->angles * dt);
        }
    }
};


struct GameWorldSettings :
    GameSettings<
        RenderSystem<GameWorldSettings>,
        TouchSystem<GameWorldSettings>,
        DraggableSystem<GameWorldSettings>,
        TransformHierarchy<GameWorldSettings>,
        RotatorSystem<GameWorldSettings>
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
        camera->AddComponent<Transform>()->Position = { 0, 0, 170 };
        camera->GetComponent<Camera>()->FieldOfView = 40;
        
        GameObject* prev = 0;
        
        for(int i=0; i< 30; i++) {
        
            GameObject* cube = CreateObject({-4,0,0});
            cube->Parent = prev;
            prev = cube;
        }
        
    }
    
    GameObject* CreateObject(Vector3 pos) {
        GameObject* cube = world.CreateObject();
        cube->AddComponent<Transform>()->Position = pos;
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({-2,0,0}, {2,1,1});
        auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::Blue();// Colour::HslToRgb(i * 10, 1, 1, 1);
        }
        cube->AddComponent<Material>();
        cube->AddComponent<Touchable>()->Click.Bind([cube] (TouchData d) {
            cube->GetComponent<Transform>()->Rotation *= Quaternion(0.1f);
        });
        cube->AddComponent<Rotator>()->angles = Vector3(1,1,2) * 0.1f;
        cube->AddComponent<Draggable>()->Movement = Draggable::MovementMode::XYPlane;
        return cube;
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