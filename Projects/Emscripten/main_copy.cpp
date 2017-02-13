#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include <iostream>

using namespace Pocket;

class Game : public GameState<Game> {
public:
    GameWorld world;
    GameObject* cube;
    float rotation;
    
    void Initialize() {
        std::cout << "Initialize" << std::endl;
    
        
        GameObject* root = world.CreateRoot();
        std::cout << "Initialize 1" << std::endl;

        root->CreateSystem<RenderSystem>();

        std::cout << "Initialize 1" << std::endl;
        
        GameObject* camera = root->CreateChild();
        camera->AddComponent<Camera>();
        camera->AddComponent<Transform>()->Position = { 0, 0, 10 };
        //camera->GetComponent<Camera>()->FieldOfView = 40;

        std::cout << "Initialize 2" << std::endl;
        
        cube = root->CreateChild();
        cube->AddComponent<Transform>();
        cube->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        cube->AddComponent<Renderable>();

        std::cout << "Initialize 3" << std::endl;
        
        rotation = 0;
    }
    
    void Update(float dt) {
        std::cout << "Update 1"<< std::endl;
        cube->GetComponent<Transform>()->Rotation = Quaternion(rotation, Vector3(1,0.2f,0.5f).Normalized());
        std::cout << "Update 2"<< std::endl;
        rotation += dt;
        std::cout << "Update 2"<< std::endl;
        world.Update(dt);
        std::cout << "Update 3"<< std::endl;
    }
    
    void Render() {
        std::cout << "Render 1"<< std::endl;
        world.Render();
        std::cout << "Render 2"<< std::endl;
    }
};

int main() {
    std::cout << "Hello world" << std::endl;
    Engine e;
    e.Start<Game>();
	return 0;
}