#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "MapMeshSystem.h"
#include "FirstPersonMoverSystem.hpp"
#include "TransformHierarchy.hpp"
#include "DragSelector.hpp"
#include "SelectionVisualizer.hpp"
#include "VelocitySystem.hpp"
#include "MoveSystem.h"
#include "PathFinderSystem.h"
#include "ClickTargetSystem.h"
#include "TouchSystem.hpp"
#include "NavMesh.hpp"
#include "Timer.hpp"
#include "ParticleUpdaterSystem.h"
#include "ParticleTransformSystem.h"
#include "ParticleMapCollisionSystem.h"
#include "ParticleCollisionSystem.h"
#include "ParticleGroundSystem.h"
#include "ParticleAirSystem.h"
#include "ObstacleSystem.h"

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
    GameObject* navMesh;
    GameObject* collisionMesh;
    
    GameObject* marker;
    
    void Initialize() {
        navMesh = 0;
        collisionMesh = 0;
        
        MathHelper::SeedRandom(2);
        
        auto fpm = world.CreateSystem<FirstPersonMoverSystem>();
        fpm->Input = &Input;
        fpm->FlipControls = true;
        world.CreateSystem<TransformHierarchy>();
        world.CreateSystem<TouchSystem>()->Input = &Input;
        world.CreateSystem<MapMeshSystem>();
        renderer = world.CreateSystem<RenderSystem>();
        renderer->Shaders.LitTextured.SetValue("LightDirection", Vector3(-1,-1,-1).Normalized());
        world.CreateSystem<DragSelector>()->Setup(Manager().Viewport(), Input);
        world.CreateSystem<SelectionVisualizer>();
        world.CreateSystem<VelocitySystem>();
        world.CreateSystem<PathFinderSystem>();
        world.CreateSystem<ClickTargetSystem>();
        
        world.CreateSystem<MoveSystem>();
        world.CreateSystem<ParticleCollisionSystem>()->AddComponent<Groundable>();
        world.CreateSystem<ParticleCollisionSystem>()->AddComponent<Airable>();
        
        world.CreateSystem<ParticleMapCollisionSystem>();
        world.CreateSystem<ParticleUpdaterSystem>();
        //world.CreateSystem<ParticleTransformSystem>();
        world.CreateSystem<ParticleGroundSystem>();
        world.CreateSystem<ParticleAirSystem>();
        world.CreateSystem<ObstacleSystem>();
        
        map = world.CreateObject();
        
        Point mapSize(256,256);
        
        map->AddComponent<Map>()->CreateMap(mapSize.x, mapSize.y);
        
        map->GetComponent<Map>()->Randomize(-6.5f, 8.5f);
        map->GetComponent<Map>()->Smooth(5);
        
        for (int i=0; i<40; i++) {
           map->GetComponent<Map>()->AddHill(MathHelper::Random(mapSize.x), MathHelper::Random(mapSize.y), 4, 5.0f);
        }
        
        for (int i=0; i<50; i++) {
           map->GetComponent<Map>()->AddHill(MathHelper::Random(mapSize.x), MathHelper::Random(mapSize.y), 6, -2.0f);
        }
        
        //map->GetComponent<Map>()->AddHill(64, 64, 40, 5);
        
        /*
        
        map->GetComponent<Map>()->SetHeight(1.0f);
        //map->GetComponent<Map>()->Randomize(-0.5f, 1.6f);
        //map->GetComponent<Map>()->Smooth(1);
        
        for (int i=0; i<60; i++) {
           map->GetComponent<Map>()->AddHill(MathHelper::Random(128), MathHelper::Random(128), MathHelper::Random(4, 6), 5.0f);
        }
        
        for (int i=0; i<60; i++) {
           map->GetComponent<Map>()->AddHill(MathHelper::Random(128), MathHelper::Random(128), MathHelper::Random(4, 6), -2.0f);
        }
        */
        
        
        /*
        map->GetComponent<Map>()->SetHeight(1.0f);
        map->GetComponent<Map>()->AddHill(32, 32, 16, 5.0f);
        map->GetComponent<Map>()->AddHill(32, 32, 4, -7.0f);
        //map->GetComponent<Map>()->AddHill(5, 5, 4, 7.0f);
        map->GetComponent<Map>()->AddHill(35, 5, 4, 7.0f);
        map->GetComponent<Map>()->AddHill(35, 45, 8, -3.0f);
        */
        //map->GetComponent<Map>()->GetNode(10, 10).height = 0;
        //map->GetComponent<Map>()->GetNode(11, 10).height = 0;
        //map->GetComponent<Map>()->GetNode(11, 11).height = 0;
        //map->GetComponent<Map>()->GetNode(10, 11).height = 0;
        
        //map->GetComponent<Map>()->SetEdges(1.0f);
        
        
        
        
        
        
        
        cameraObject = world.CreateObject();
        cameraObject->AddComponent<Transform>()->Position = {0,0,0};
        cameraObject->AddComponent<Mesh>();
        cameraObject->AddComponent<MapRenderer>()->width = 70;
        cameraObject->GetComponent<MapRenderer>()->depth = 48;
        cameraObject->AddComponent<Map>(map);
        cameraObject->AddComponent<TextureComponent>()->Texture().LoadFromPng("grass.png");
        cameraObject->AddComponent<Material>()->Shader = &renderer->Shaders.LitTextured;
        cameraObject->AddComponent<FirstPersonMover>()->RotationSpeed = 0;
        cameraObject->AddComponent<Touchable>();
        
        camera = world.CreateObject();
        camera->Parent = cameraObject;
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        camera->AddComponent<Transform>()->Position = Vector3(0, 10, 5) * 1.8f;
        camera->GetComponent<Transform>()->Rotation = Quaternion::LookAt(camera->GetComponent<Transform>()->Position, {0,0,0}, Vector3(0,1,0));
        camera->GetComponent<Camera>()->FieldOfView = 70;
        
        GameObject* waterPlane = world.CreateObject();
        waterPlane->Parent = cameraObject;
        waterPlane->AddComponent<Transform>()->Position = {0,0.1f,0};
        waterPlane->AddComponent<Mesh>()->GetMesh<Vertex>().AddPlane(0, {76, 48}, {0,0,1,1});
        waterPlane->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour(0,0,1.0f,0.5f));
        waterPlane->AddComponent<Material>()->BlendMode = BlendModeType::Alpha;
        waterPlane->GetComponent<Material>()->Shader = &renderer->Shaders.Colored;
        
        for (int i=0; i<100; i++) {
        Vector2 position ={15.0f+(i%10)*2.0f,20.0f + floorf(i/10) *2.0f};
        cube = world.CreateObject();
        cube->AddComponent<Mappable>()->Map = map->GetComponent<Map>();
        cube->AddComponent<Transform>()->Position ={position.x,1.0f, position.y};
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({0,0.2f,0}, {0.55f,0.2f,1.0f});
        cube->AddComponent<Material>()->Shader = &renderer->Shaders.LitColored;
        cube->AddComponent<Selectable>();
        cube->AddComponent<Movable>();
        cube->AddComponent<Particle>()->SetPosition(position);
        if (i>50) {
            cube->AddComponent<Airable>()->targetAboveGround = 4.0f;
            cube->GetComponent<Airable>()->heightAligmentSpeed = 2.0f;
            cube->GetComponent<Airable>()->alignmentSpeed = 2.0f;
            cube->GetComponent<Movable>()->Speed = 6.0f;
            //cube->GetComponent<Movable>()->Acc = 5.0f;
        } else {
            cube->AddComponent<Groundable>()->alignmentSpeed = 10.0f;
            //cube->GetComponent<Movable>()->Acc = 10.0f;
            cube->GetComponent<Movable>()->Speed = 2.0f;
        }
        GameObject* turret = world.CreateObject();
        turret->Parent = cube;
        turret->AddComponent<Transform>()->Position = {0,0.4f,0};
        turret->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({0.0f,0,0.5f}, {0.04f, 0.04f, 0.8f});
        turret->AddComponent<Material>()->Shader = &renderer->Shaders.LitColored;
        
        //if (i==5) {
        //    cube->GetComponent<Particle>()->immovable = true;
        //}
        
        }
        
        for (int i=0; i<30; i++) {
            Point size(1 + MathHelper::Random(5), 1 + MathHelper::Random(5));
            GameObject* building = world.CreateObject();
            building->AddComponent<Transform>()->Position = {(float)MathHelper::Random(128),1.0f,(float)MathHelper::Random(128)};
            building->GetComponent<Transform>()->Rotation = Quaternion(MathHelper::Random(0, MathHelper::DegToRad * 360.0f), {0,1,0});
            building->AddComponent<Obstacle>()->size = size;
            building->AddComponent<Material>()->Shader = &renderer->Shaders.LitColored;
            building->AddComponent<Mappable>()->Map = map->GetComponent<Map>();
            auto& mesh = building->AddComponent<Mesh>()->GetMesh<Vertex>();
            mesh.AddCube({0,1,0}, {(float)size.x,1,(float)size.y});
        }
        
        
        marker= world.CreateObject();
        marker->AddComponent<Transform>()->Position = {155,0,155};
        marker->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 0.4f);
        marker->AddComponent<Material>();
    
        follow = false;
    
        Input.ButtonDown += event_handler(this, &Game::ButtonDown);
        cameraObject->GetComponent<Touchable>()->Down += event_handler(this, &Game::TerrainDown);
        wireframe = false;
        
        map->GetComponent<Map>()->NavigationUpdated += event_handler(this, &Game::NavigationMeshUpdated);
    }
    
    void NavigationMeshUpdated(Map* map) {
        if (navMesh) navMesh->Remove();
        if (collisionMesh) collisionMesh->Remove();
        
    
        NavMesh& mesh = *map->NavMesh();
        
        for (int i=0; i<2; i++) {
        
            GameObject* meshObject = world.CreateObject();
            meshObject->AddComponent<Transform>();
            meshObject->AddComponent<Material>()->Shader = &renderer->Shaders.Colored;
            meshObject->GetComponent<Material>()->BlendMode = BlendModeType::Alpha;
            meshObject->EnableComponent<Material>(false);
            auto& navMeshMesh = meshObject->AddComponent<Mesh>()->GetMesh<Vertex>();
            
            NavMesh::Vertices& vertices = i==0 ? mesh.navigation : mesh.collision;
            
            for(auto& p : vertices) {
                Vertex v;
                v.Position = {p.x, 1.50f+i*0.1f, p.y};
                v.Color = i==0 ? Colour::Red() : Colour::Green(); //Colour::HslToRgb(i*180, 1, 1, 1);
                navMeshMesh.vertices.push_back(v);
                if (navMeshMesh.vertices.size()>25000) break;
            }
            
            for (auto& p : mesh.GetTriangles()) {
                bool valid = true;
                for (int i=0; i<3; i++) {
                    if (p->corners[i]>=navMeshMesh.vertices.size()) {
                        valid = false;
                        break;
                    }
                }
                if (!valid) continue;
            
                for (int i=0; i<3; i++) {
                    navMeshMesh.triangles.push_back(p->corners[i]);
                }
            }
            navMeshMesh.Flip();
            
            if (i==0) {
                navMesh = meshObject;
            } else {
                collisionMesh = meshObject;
            }
        }
    }
    
    void TerrainDown(TouchData d) {
        
        Vector2 pos = { d.WorldPosition.x, d.WorldPosition.z};
        
        Map* map = cameraObject->GetComponent<Map>();
        if (!map->NavMesh()) return;
        
        Vector2 nearestPosition;
        int version = 0;
        map->NavMesh()->FindNearestTriangle(map->NavMesh()->navigation, pos, nearestPosition, version);
        
        marker->GetComponent<Transform>()->Position = {nearestPosition.x, 1.5f, nearestPosition.y};
    }
    
    bool follow;
    
    void ButtonDown(std::string b) {
        if (b=="f") {
            follow = !follow;
        } else if (b=="w") {
            wireframe = !wireframe;
        } else if (b=="n") {
            navMesh->EnableComponent<Material>(!navMesh->IsComponentEnabled<Material>());
        }else if (b=="c") {
            collisionMesh->EnableComponent<Material>(!collisionMesh->IsComponentEnabled<Material>());
        } else if (b=="b") {
            Point size(1 + MathHelper::Random(3), 1 + MathHelper::Random(3));
            GameObject* building = world.CreateObject();
            building->AddComponent<Transform>()->Position = marker->GetComponent<Transform>()->Position;
            building->GetComponent<Transform>()->Rotation = Quaternion(MathHelper::Random(0, MathHelper::DegToRad * 360.0f), {0,1,0});
            building->AddComponent<Obstacle>()->size = size;
            building->AddComponent<Material>()->Shader = &renderer->Shaders.LitColored;
            building->AddComponent<Mappable>()->Map = map->GetComponent<Map>();
            auto& mesh = building->AddComponent<Mesh>()->GetMesh<Vertex>();
            mesh.AddCube({0,1,0}, {(float)size.x,1,(float)size.y});
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
        if (wireframe) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        } else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        world.Render();
    }
    
    bool wireframe;
};

int main() {
    Engine e;
    e.Start<Game>();
    //e.Start<Game>(1440, 900, true);
    return 0;
}