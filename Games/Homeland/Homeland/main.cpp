#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "MapMeshSystem.h"
#include "FirstPersonMoverSystem.hpp"
#include "TransformHierarchy.hpp"

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
        
        
        world.CreateSystem<FirstPersonMoverSystem>()->Input = &Input;
        world.CreateSystem<TransformHierarchy>();
        world.CreateSystem<MapMeshSystem>();
        renderer = world.CreateSystem<RenderSystem>();
        renderer->Shaders.LitTextured.SetValue("LightDirection", Vector3(-1,-1,-1).Normalized());
        
        map = world.CreateObject();
        map->AddComponent<Map>()->CreateMap(512, 512);
        map->GetComponent<Map>()->Randomize(-15.0f, 15.0f);
        map->GetComponent<Map>()->Smooth(3);
        
        cameraObject = world.CreateObject();
        cameraObject->AddComponent<Transform>()->Position = {0,0,0};
        cameraObject->AddComponent<Mesh>();
        cameraObject->AddComponent<MapRenderer>()->width = 76;
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
        waterPlane->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
        waterPlane->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour(0,0,1.0f,0.5f));
        waterPlane->GetComponent<Material>()->Shader = &renderer->Shaders.Colored;
        
        cube = world.CreateObject();
        cube->AddComponent<Transform>()->Position = {512,0,512};
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddGeoSphere(0, 4, 3);//({0,0,5.0f}, {1,0.5f,5.0f});
        cube->AddComponent<Material>()->Shader = &renderer->Shaders.LitColored;
        
        auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        
        for (int i=0; i<verts.size(); i++) {
        //    verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
        }
        
        
        rotation = 0;
    }
    
    void Update(float dt) {
        //cube->GetComponent<Transform>()->Rotation = Quaternion(rotation, Vector3(0,1,0));
        rotation += dt;
        
        //Vector3 pos = camera->GetComponent<Transform>()->Position;
        //pos.y = 0;
        //cameraObject->GetComponent<Transform>()->Position = pos;
        
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