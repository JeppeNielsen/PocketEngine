#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "triangle.h"
#include "MathHelper.hpp"
#include "ParticleUpdaterSystem.h"
#include "NavMesh.hpp"
#include "ParticleTransformSystem.h"

using namespace Pocket;


class TestOutOfBounds : public GameState<TestOutOfBounds> {
public:
    GameWorld world;
    bool wireframe;
    NavTriangle* triangle;
    GameObject* particle;
    
    void Initialize() {
        world.CreateSystem<RenderSystem>();
        world.CreateSystem<ParticleUpdaterSystem>();
        world.CreateSystem<ParticleTransformSystem>();
        
        GameObject* camera = world.CreateObject();
        camera->AddComponent<Transform>()->Position = {0,30,80};
        camera->AddComponent<Camera>();
        
        particle = world.CreateObject();
        particle->AddComponent<Particle>()->position = {-10,30};
        particle->AddComponent<Transform>();
        particle->AddComponent<Material>();
        particle->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        /*
        triangle = new NavTriangle();
        triangle->corners[0]=Vector2(0,0);
        triangle->corners[2]=Vector2(-50,50);
        triangle->corners[1]=Vector2(50,50);
        
        GameObject* go = world.CreateObject();
        go->AddComponent<Transform>();
        go->AddComponent<Material>();
        auto& mesh = go->AddComponent<Mesh>()->GetMesh<Vertex>();
        for (int i=0; i<3; i++) {
            Vertex v;
            v.Position = triangle->corners[i];
            v.Color = Colour::Red();
            mesh.vertices.push_back(v);
        }
        mesh.triangles.push_back(0);
        mesh.triangles.push_back(1);
        mesh.triangles.push_back(2);
        
        */
        
        
        Input.ButtonDown.Bind(this, &TestOutOfBounds::ButtonDown);
        wireframe = false;
    }
    
    void ButtonDown(std::string b) {
        if (b=="w") {
            wireframe = !wireframe;
        }
    }
    
    void Update(float dt) {
        particle->GetComponent<Particle>()->position += Vector2(0,-10 * dt);
        Vector2 position;
        //triangle->GetDistance( particle->GetComponent<Particle>()->position, position);
        //particle->GetComponent<Particle>()->position = position;
        
        world.Update(dt);
    }
    
    void Render() {
        if (wireframe) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        } else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        world.Render();
    }
};

int main_wolla() {
    Engine e;
    e.Start<TestOutOfBounds>();
	return 0;
}