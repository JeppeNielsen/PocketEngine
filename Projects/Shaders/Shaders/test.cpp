#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"

using namespace Pocket;

class Test : public GameState<Test> {

    GameWorld world;
    GameObject* cube;
    
    void Initialize() {
        auto renderer = world.CreateSystem<RenderSystem>();
        renderer->DefaultShader = &renderer->Shaders.LitColored;
        renderer->DefaultTexturedShader = &renderer->Shaders.LitTextured;
        
        GameObject* cameraObject = world.CreateObject();
        cameraObject->AddComponent<Transform>()->Position = {0,0,8};
        cameraObject->AddComponent<Camera>()->Viewport = Manager().Viewport();

        Box viewPort = Manager().Viewport();
        
        cube = world.CreateObject();
        cube->AddComponent<Transform>();
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube->AddComponent<Material>();
        cube->AddComponent<TextureComponent>()->Texture().LoadFromPng("Football.png");
        cube->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour::HslToRgb(80, 1.0, 1.0, 1.0));
        
        
        GameObject* cube2 = world.CreateObject();
        cube2->AddComponent<Transform>()->Position = {3,0,0};
        cube2->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube2->AddComponent<Material>();
        //cube2->AddComponent<TextureComponent>(cube);
        cube2->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour::HslToRgb(145, 1.0, 1.0, 1.0));
        
        

        GameObject* cube3 = world.CreateObject();
        cube3->AddComponent<Transform>()->Position = {-3,0,0};
        cube3->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube3->AddComponent<Material>();
        cube3->AddComponent<TextureComponent>(cube);
        cube3->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour::HslToRgb(60, 1.0, 1.0, 1.0));
        
        GameObject* cube4 = world.CreateObject();
        cube4->AddComponent<Transform>()->Position = {-6,0,0};
        cube4->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube4->AddComponent<Material>();
        cube4->AddComponent<TextureComponent>(cube);
        
    }

    void Update(float dt) {
        static float rot = 0;
        rot += dt;
        cube->GetComponent<Transform>()->Rotation = Quaternion(rot, Vector3(0.3f,1,0.6f).Normalized());
        world.Update(dt);
    }

    void Render() {
        glClearColor(0,0,0.3f,1);
        glClear(GL_COLOR_BUFFER_BIT);
        world.Render();
    }
};

int main() {
    Engine e;
    e.Start<Test>();
    return 0;
}
