#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "PhysicsSystem2d.hpp"
#include "FirstPersonMoverSystem.hpp"
#include "CharacterControllerSystem.hpp"
#include "GamePadSystem.hpp"
#include "GroundedSystem.hpp"

using namespace Pocket;

class Game : public GameState<Game> {
public:
    GameWorld world;
    RenderSystem* renderer;
    GameObject* camera;
    GameObject* cube;
    float rotation;
    
    void Initialize() {
        
        renderer = world.CreateSystem<RenderSystem>();
        world.CreateSystem<FirstPersonMoverSystem>()->Input = &Input;
        
        world.CreateSystem<GamePadSystem>()->SetInput(&Input);
        world.CreateSystem<CharacterControllerSystem>();
        world.CreateSystem<PhysicsSystem2d>();
        world.CreateSystem<GroundedSystem>();
        
        
        camera = world.CreateObject();
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        camera->AddComponent<Transform>()->Position = { 0, 4, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 60;
        //camera->GetComponent<Camera>()->Orthographic = true;
        //camera->GetComponent<Camera>()->Viewport = {-25,17,15,-27};
        camera->AddComponent<FirstPersonMover>();
        
        
        GameObject* floor = world.CreateObject();
        floor->AddComponent<Transform>();
        floor->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, {50,0.1f,50});
        floor->AddComponent<Material>();
        auto& floorPolygon = floor->AddComponent<RigidBody2d>()->CreatePolygon(0,0.2f,0);
        floorPolygon.SetAsBox(100, 0.1f);
        
        
        GameObject* hero = CreateCube({0,10},{0.5f,1.0f}, 1.0f);
        hero->GetComponent<RigidBody2d>()->bodyDefinition.fixedRotation = true;
        hero->AddComponent<CharacterController>();
        hero->AddComponent<GamePadController>()->gamePadIndex = 0;
        hero->AddComponent<Groundable>();
        hero->AddComponent<Collidable>();
        
        CreateCube({0.15f,6},{0.5f,1.0f},1.0f);
        
        GameObject* obj = CreateCube({10.0f,5},{10.0f,0.5f},0.0f);
        
        obj->GetComponent<Transform>()->Rotation = Quaternion(0, 0, 0.6f);
        
        
        rotation = 0;
    }
    
    GameObject* CreateCube(Vector2 pos, Vector2 size, float density) {
        cube = world.CreateObject();
        cube->AddComponent<Transform>()->Position = pos;
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, {size.x,size.y,1});
        cube->AddComponent<Material>();
        cube->AddComponent<RigidBody2d>()->CreatePolygon(density,0.2f,0.0f).SetAsBox(size.x, size.y);
        
        VertexMesh<Vertex>::Vertices& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
        }
    
        return cube;
    }
    
    void Update(float dt) {
        //cube->GetComponent<Transform>()->Rotation = Quaternion(rotation, Vector3(0.9f,0.2f,0.5f).Normalized());
        rotation += dt;
        world.Update(dt);
    }
    
    void Render() {
        renderer->Render();
    }
};


int main_old() {
    Engine e;
    e.Start<Game>();
	return 0;
}
