#include "Engine.hpp"
#include "GameState.hpp"
#include "OpenGL.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "FirstPersonMoverSystem.hpp"

using namespace Pocket;

class Test : public GameState<Test> {

    GameWorld world;
    GameObject* cube;
    
    void Initialize() {
        auto renderer = world.CreateSystem<RenderSystem>();
        renderer->DefaultShader = &renderer->Shaders.LitColored;
        
        world.CreateSystem<FirstPersonMoverSystem>()->Input = &Input;

        GameObject* cameraObject = world.CreateObject();
        cameraObject->AddComponent<Transform>()->Position = {0,0,5};
        cameraObject->AddComponent<FirstPersonMover>();

        auto viewPort = Context().Viewport();

        cube = world.CreateObject();
        cube->AddComponent<Transform>();
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube->AddComponent<Material>();
        cube->AddComponent<TextureComponent>()->Texture().LoadFromPng("Bird.png");
        
        Input.TouchDown.Bind(this, &Test::TouchDown);
    }
    
    void TouchDown(TouchEvent e) {
        rotate = !rotate;
    }
    
    bool rotate = false;

    void Update(float dt) {
        static float rot = 0;
        rot += dt;
        cube->GetComponent<Transform>()->Rotation = Quaternion(rot, Vector3(0.3f,1,0.6f).Normalized());
        world.Update(dt);
    }

    void Render() {
        glClearColor(0,0,1,1);
        glClear(GL_COLOR_BUFFER_BIT);
        world.Render();
    }
};

int main_nono() {
    Engine e;
    e.Start<Test>();
    return 0;
}
