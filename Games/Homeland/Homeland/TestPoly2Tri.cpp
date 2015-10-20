#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "clipper.hpp"
#include "clip2tri.h"
//#include "gpc.h"
#include "tesselator.h"
#include "poly2tri.h"

using namespace Pocket;


class TestPoly2Tri : public GameState<TestPoly2Tri> {
public:
    GameWorld world;
    bool wireframe;
    
    void Initialize() {
        RenderSystem* renderSystem = world.CreateSystem<RenderSystem>();
        
        GameObject* camera = world.CreateObject();
        camera->AddComponent<Transform>()->Position = {0,0,20};
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        
        /*
        vector<vector<c2t::Point>> inputPolygons;
        
        for (int i=0; i<10; i++) {
        Vector2 p = {10 + i * 10.0f, 10};
        Vector2 s = {8.0f, 20.0f};
        inputPolygons.push_back({
            {p.x,p.y}, {p.x+s.x,p.y}, {p.x+s.x,p.y+s.y}, {p.x,p.y+s.y},
            {p.x + s.x * 0.5f, p.y + s.y * 0.5f }
        });
        }
        
        vector<c2t::Point> outer { { 0,0 }, {0,100}, {100,100}, {100,0}};
        
        c2t::clip2tri clipper;
        
        vector<c2t::Point> navigationMesh;
        
        clipper.triangulate(inputPolygons, navigationMesh, outer);
        
        GameObject* cube = world.CreateObject();
        cube->AddComponent<Transform>();
        auto& mesh = cube->AddComponent<Mesh>()->GetMesh<Vertex>();
        
        int i=0;
        for (auto p : navigationMesh) {
            Vertex v;
            v.Position = {p.x, p.y,0};
            v.Color = Colour::White();
            mesh.vertices.push_back(v);
            mesh.triangles.push_back(i);
            i++;
        }
        
        cube->AddComponent<Material>();
        */
        
        
        
        
        
        std::vector<p2t::Point*> outer;
        
        outer.push_back(new p2t::Point(0,0));
        outer.push_back(new p2t::Point(1000,0));
        outer.push_back(new p2t::Point(1000,1000));
        outer.push_back(new p2t::Point(0,1000));
        
        p2t::CDT clipper(outer);
        
        std::vector<p2t::Point*> hole;
        
        hole.push_back(new p2t::Point(250,250));
        hole.push_back(new p2t::Point(250,750));
        hole.push_back(new p2t::Point(750,750));
        hole.push_back(new p2t::Point(750,250));
        
        clipper.Triangulate();
        
        auto triangles = clipper.GetTriangles();
        
        GameObject* go = world.CreateObject();
        go->AddComponent<Transform>();
        go->AddComponent<Material>();
        auto& mesh = go->AddComponent<Mesh>()->GetMesh<Vertex>();
        
        int indices = 0;
        for (int i=0; i<triangles.size(); i++) {
            auto* tri = triangles[i];
            
            //tri->GetPoint(0);
            for (int j=0; j<3; j++) {
                auto* p = tri->GetPoint(j);
                Vertex v;
                v.Position = {(float)p->x*0.01f, (float)p->y*0.01f, 0};
                v.Color = Colour::White();
                mesh.vertices.push_back(v);
                mesh.triangles.push_back(indices++);
            }
        }
        
        
        Input.ButtonDown += event_handler(this, &TestPoly2Tri::ButtonDown);
        wireframe = false;
    }
    
    
    void ButtonDown(std::string b) {
        if (b=="w") {
            wireframe = !wireframe;
        }
    }

    
    void Update(float dt) {
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

int main_Ergefdvdbfdf() {
    Engine e;
    e.Start<TestPoly2Tri>();
	return 0;
}