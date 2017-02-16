#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"

using namespace Pocket;

class Game : public GameState<Game> {
public:
    GameWorld world;
    GameObject* root;
    GameObject* camera;
    GameObject* cube;
    float rotation;
    
    void Initialize() {
    
        root = world.CreateRoot();
        
        root->CreateSystem<RenderSystem>();
        
        camera = root->CreateObject();
        camera->AddComponent<Camera>();
        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
        camera->GetComponent<Camera>()->FieldOfView = 40;
        
        cube = root->CreateObject();
        cube->AddComponent<Transform>();
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube->AddComponent<Renderable>();
        
        auto& verts = cube->GetComponent<Mesh>()->GetMesh<Vertex>().vertices;
        
        for (int i=0; i<verts.size(); i++) {
            verts[i].Color = Colour::HslToRgb(i * 10, 1, 1, 1);
        }
        
        rotation = 0;
    }
    
    void Update(float dt) {
        cube->GetComponent<Transform>()->Rotation = Quaternion(rotation, Vector3(1,0.2f,0.5f).Normalized());
        rotation += dt;
        world.Update(dt);
    }
    
    void Render() {
        world.Render();
    }
};

int main_nono() {
    Engine e;
    e.Start<Game>();
	return 0;
}
