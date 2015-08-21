#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "MapMeshSystem.h"
#include "FirstPersonMoverSystem.hpp"
#include "TransformHierarchy.hpp"
#include "DragSelector.hpp"
#include "SelectionVisualizer.hpp"
#include "VelocitySystem.hpp"
#include "GroundSystem.h"
#include "MoveSystem.h"
#include "PathFinderSystem.h"

using namespace Pocket;

class Game : public GameState<Game> {
public:
    GameWorld world;
    RenderSystem* renderer;
    GameObject* camera;
    GameObject* cube;
    float rotation;
    GameObject* map;
    GameObject* cameraObject;
    
    
    void Initialize() {
        
        auto fpm = world.CreateSystem<FirstPersonMoverSystem>();
        fpm->Input = &Input;
        fpm->FlipControls = true;
        world.CreateSystem<TransformHierarchy>();
        world.CreateSystem<MapMeshSystem>();
        renderer = world.CreateSystem<RenderSystem>();
        renderer->Shaders.LitTextured.SetValue("LightDirection", Vector3(-1,-1,-1).Normalized());
        world.CreateSystem<DragSelector>()->Setup(Manager().Viewport(), Input);
        world.CreateSystem<SelectionVisualizer>();
        world.CreateSystem<VelocitySystem>();
        world.CreateSystem<PathFinderSystem>();
        world.CreateSystem<MoveSystem>();
        world.CreateSystem<GroundSystem>();
       
        
        map = world.CreateObject();
        map->AddComponent<Map>()->CreateMap(164, 164);
        map->GetComponent<Map>()->Randomize(-13.1f, 15.0f);
        map->GetComponent<Map>()->Smooth(3);
        map->GetComponent<Map>()->SetMaxHeight(0.5f);
        map->GetComponent<Map>()->AddHill(32, 32, 16, 5.0f);
        map->GetComponent<Map>()->AddHill(32, 32, 4, -7.0f);
        //map->GetComponent<Map>()->AddHill(0, 0, 12, 7.0f);
        
        
        
        cameraObject = world.CreateObject();
        cameraObject->AddComponent<Transform>()->Position = {0,0,0};
        cameraObject->AddComponent<Mesh>();
        cameraObject->AddComponent<MapRenderer>()->width = 70;
        cameraObject->GetComponent<MapRenderer>()->depth = 48;
        cameraObject->AddComponent<Map>(map);
        cameraObject->AddComponent<TextureComponent>()->Texture().LoadFromPng("grass.png");
        cameraObject->AddComponent<Material>()->Shader = &renderer->Shaders.LitTextured;
        cameraObject->AddComponent<FirstPersonMover>()->RotationSpeed = 0;
        
        camera = world.CreateObject();
        camera->Parent = cameraObject;
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        camera->AddComponent<Transform>()->Position = Vector3(0, 10, 5) * 1.5f;
        camera->GetComponent<Transform>()->Rotation = Quaternion::LookAt(camera->GetComponent<Transform>()->Position, {0,0,0}, Vector3(0,1,0));
        camera->GetComponent<Camera>()->FieldOfView = 70;
        
        GameObject* waterPlane = world.CreateObject();
        waterPlane->Parent = cameraObject;
        waterPlane->AddComponent<Transform>()->Position = {0,0.1f,0};
        waterPlane->AddComponent<Mesh>()->GetMesh<Vertex>().AddPlane(0, {76, 48}, {0,0,1,1});
        waterPlane->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour(0,0,1.0f,0.5f));
        waterPlane->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
        waterPlane->GetComponent<Material>()->Shader = &renderer->Shaders.Colored;
        
        cube = world.CreateObject();
        cube->AddComponent<Mappable>()->Map = map->GetComponent<Map>();
        cube->AddComponent<Transform>()->Position = {15.0f,1.0f,20.0f};
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, {0.95f,0.2f,1.0f});
        cube->AddComponent<Material>()->Shader = &renderer->Shaders.LitColored;
        cube->AddComponent<Selectable>();
        cube->AddComponent<Groundable>();
        auto& path = cube->AddComponent<Movable>()->path;
        path.push_back({20,0,20});
        path.push_back({0,0,0});
        path.push_back({0,0,20});
        path.push_back({20,0,0});
        cube->GetComponent<Movable>()->Speed = 4.0f;
        
        cube->GetComponent<Transform>()->Rotation = Quaternion(0.5f, {0,1,0});
        
        GameObject* obj = world.CreateObject();
        obj->AddComponent<Transform>()->Position = {155,0,155};
        obj->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        obj->AddComponent<Material>();
    
        follow = false;
    
        Input.ButtonDown += event_handler(this, &Game::ButtonDown);
    }
    
    bool follow;
    
    void ButtonDown(std::string b) {
        if (b == "m") {
            cube->GetComponent<Movable>()->Target = {155,0,155};
        } else if (b == "n") {
            cube->GetComponent<Movable>()->Target = {15,0,20};
        } else if (b=="f") {
            follow = !follow;
        }
    }
    
    void Update(float dt) {
        world.Update(dt);
        if (follow) {
            Vector3 pos = cube->GetComponent<Transform>()->Position;
            pos.y = 0;
            cameraObject->GetComponent<Transform>()->Position = pos;
        }
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