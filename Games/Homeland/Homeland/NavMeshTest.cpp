//
//  NavMeshTest.cpp
//  Homeland
//
//  Created by Jeppe Nielsen on 19/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"
#include "GameWorld.hpp"
#include "RenderSystem.hpp"
#include "NavMesh.hpp"
#include "FirstPersonMoverSystem.hpp"

using namespace Pocket;

class NavMeshTest : public GameState<NavMeshTest> {
public:
    GameWorld world;
    RenderSystem* renderer;
    GameObject* camera;
    NavMesh navMesh;
    bool wireFrame;
    std::vector<Vector2> hole;
    
    void Initialize() {
        
        auto fpm = world.CreateSystem<FirstPersonMoverSystem>();
        fpm->Input = &Input;
        fpm->FlipControls = true;
        
        renderer = world.CreateSystem<RenderSystem>();
        renderer->Shaders.LitTextured.SetValue("LightDirection", Vector3(-1,-1,-1).Normalized());
        
        camera = world.CreateObject();
        camera->AddComponent<Transform>()->Position = {0,0,30};
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        
        std::vector<Vector2> points {
            { 0,0 }, {0,10}, {10,0},
            //{ 5,0 }, {5,5}, {0,5},
            //{ 5,0 }, {10,0}, {5,5},
            //{ 10,0 }, {10,5}, {5,5},
            //{ 5,5 }, {10,5}, {10,10},
        };

        navMesh.Build(points);
        
        hole.push_back({-2,5});
        hole.push_back({10,7.5f});
        hole.push_back({10,2.5f});
        
        /*
        Vector2 start(0.5f,4);
        Vector2 end(9.5f,8);

        NavTriangle* startTri = navMesh.FindTriangle(start);
        NavTriangle* endTri = navMesh.FindTriangle(end);
        
        auto path = navMesh.FindPath(startTri, start, endTri, end);
        auto straightPath = navMesh.FindStraightPath(path);
        */
        
        
        
        {
            GameObject* go = world.CreateObject();
            go->AddComponent<Transform>();
            go->AddComponent<Material>()->Shader = &renderer->Shaders.Colored;
            auto& mesh = go->AddComponent<Mesh>()->GetMesh<Vertex>();
            
            for (int i=0; i<points.size(); i++) {
                Vertex v;
                v.Position = {points[i].x, points[i].y, 0};
                v.Color = Colour::White();
                mesh.vertices.push_back(v);
                mesh.triangles.push_back(i);
            }
            
            mesh.Flip();
        }
        
        /*
        if (false)
        {
            GameObject* go = world.CreateObject();
            go->AddComponent<Transform>();
            go->AddComponent<Material>()->Shader = &renderer->Shaders.Colored;
            auto& mesh = go->AddComponent<Mesh>()->GetMesh<Vertex>();
            
            for (int i=0; i<straightPath.size(); i++) {
                Vertex v;
                v.Position = {straightPath[i].x, straightPath[i].y, 0.2f};
                v.Color = Colour::Blue();
                mesh.vertices.push_back(v);
                
                Vertex v2;
                v2.Position = {straightPath[i].x, straightPath[i].y+0.05f, 0.2f};
                v2.Color = Colour::Blue();
                mesh.vertices.push_back(v2);
                
                
            }
            
            for (int i=0; i<straightPath.size()-1; i++) {
                int index = i*2;
                mesh.triangles.push_back(index+0);
                mesh.triangles.push_back(index+2);
                mesh.triangles.push_back(index+3);
                
                mesh.triangles.push_back(index+0);
                mesh.triangles.push_back(index+3);
                mesh.triangles.push_back(index+1);
                
            }
        }
        */
        
        
        {
            
            auto cut = navMesh.Cut(hole);
        
            GameObject* go = world.CreateObject();
            go->AddComponent<Transform>();
            go->AddComponent<Material>()->Shader = &renderer->Shaders.Colored;
            auto& mesh = go->AddComponent<Mesh>()->GetMesh<Vertex>();
            
            for (int i=0; i<cut.size(); i++) {
                Vertex v;
                v.Position = {cut[i].x, cut[i].y, 0.4f};
                v.Color = Colour::Green();
                mesh.vertices.push_back(v);
                mesh.triangles.push_back(i);
            }
            
           // mesh.Flip();
        }
        
       
        Input.ButtonDown += event_handler(this, &NavMeshTest::ButtonDown);
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
    
    bool wireframe;
};

int main_old_nonon() {
    Engine e;
    e.Start<NavMeshTest>();
	return 0;
}