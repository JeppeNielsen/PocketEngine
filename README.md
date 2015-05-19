# PocketEngine
Game Engine in c++

Currently supports MacOS, iOS, Web (via Emscripten)

Work in progress Game called Cubism:

www.pocketgames.dk/cubism

PocketTris:

www.pocketgames.dk/pocket/examples/PocketTris.html

See web example here:

www.pocketgames.dk/pocket/examples/spinningcube.html

Produced by the following code:
```
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
    float rotation;
    
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
        
        rotation = 0;
    }
    
    void Update(float dt) {
        cube->GetComponent<Transform>()->Rotation = Quaternion(rotation, Vector3(1,0.2f,0.5f).Normalized());
        rotation += dt;
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
```
