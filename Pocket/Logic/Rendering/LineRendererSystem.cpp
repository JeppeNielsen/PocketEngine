//
//  LineMeshSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 11/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "LineRendererSystem.hpp"
#include "RenderSystem.hpp"
#include "Engine.hpp"
#include "Colorable.hpp"

using namespace Pocket;

void LineRendererSystem::Initialize() {
    renderSystem = root->GetSystem<RenderSystem>();

    storage.AddSystemType<RenderSystem>();
    world.Initialize(storage);
    GameObject* root = world.CreateScene();
    
    BoundingBox bounds(0, Vector3(3000, 3000, 3000.0f));
    
    root->GetSystem<RenderSystem>()->Octree().SetWorldBounds(bounds);
    root->AddComponent<Transform>();
    lineMesh = root->AddComponent<Mesh>();
    root->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    
    GameObject* camera = root->CreateObject();
    camera->AddComponent<Transform>()->Position = Vector3(0,0,1);
    Camera* cam = camera->AddComponent<Camera>();
    cam->Orthographic = true;
    cam->FieldOfView = 0;
    cam->Near = 1.0f;
    cam->Far = 2.0f;
}

void LineRendererSystem::Update(float dt) {

    auto cameras = renderSystem->GetCameras();
    if (cameras->Objects().empty()) return;
    auto& mesh = lineMesh->GetMesh<Vertex>();
    mesh.Clear();

    auto& v = mesh.vertices;
    auto& t = mesh.triangles;
    
    //Vector2 screenSize = Engine::Context().ScreenSize;
    
    for(auto cameraObject : cameras->Objects()) {
        Camera* camera = cameraObject->GetComponent<Camera>();
        Transform* cameraTransform = cameraObject->GetComponent<Transform>();
        Vector3 cameraForward = cameraTransform->World().TransformVector({0,0,-1});
        Vector3 cameraPosition = cameraTransform->World().TransformPosition(0);
        
        for(GameObject* o : Objects()) {
            LineRenderer& line = *o->GetComponent<LineRenderer>();
            Transform& transform = *o->GetComponent<Transform>();
            float width = line.Width() * 0.5f;
        
            for(int i=0; (i+2)<=line.vertices.size(); i+=2) {
                Vector3 w1 = transform.World().TransformPosition(line.vertices[i]);
                Vector3 c1 = cameraTransform->WorldInverse().TransformPosition(w1);
                
                Vector3 w2 = transform.World().TransformPosition(line.vertices[i+1]);
                Vector3 c2 = cameraTransform->WorldInverse().TransformPosition(w2);
                
                if (c1.z>0 && c2.z>0) continue;
                float camNear = -camera->Near() - 0.0001f;
                
                if (c1.z>camNear) {
                    Vector3 v = c1 - c2;

                    float p = (camNear-c2.z) / v.z;
                    
                    c1 = c2 + v * p;
                    
                } else if (c2.z>camNear) {
                    Vector3 v = c2 - c1;
                    
                    // c1 = c2 + p * v;
                    // 0 = c2 + p * v;
                    //-c2 / v = p
                    
                    float p = (camNear-c1.z) / v.z;
                    
                    c2 = c1 + v * p;
                }

                //c1 = cameraTransform->World().TransformPosition(c1);
                //c2 = cameraTransform->World().TransformPosition(c2);
                                
                Vector3 v1 = camera->TransformViewPositionToScreenSpace(cameraTransform, c1);
                Vector3 v2 = camera->TransformViewPositionToScreenSpace(cameraTransform, c2);
                /*
                Vector3 v1 = camera->TransformPointToScreenSpace(cameraTransform, worldPosition);
                
                worldPosition = transform.World().TransformPosition(line.vertices[i+1]);
                Vector3 v2 = camera->TransformPointToScreenSpace(cameraTransform, worldPosition);
                
                if (v1.z<0 && v2.z<0) continue;
                
                if (v1.z<0) {
                    Vector3 v = v1 - v2;
                    // z = z_o + dz*p;
                    // -1 =
                    // -1-z_0 / dz = p;
                    
                    float p = v2.z / v.z;
                    
                    v1 = v2 + v * p;
                } else if (v2.z<0) {
                    Vector3 v = v2 - v1;
                    // z = z_o + dz*p;
                    // -z_0 / dz = p;
                    
                    float p = v1.z / v.z;
                    
                    v2 = v1 + v * p;
                }
                */
                
                //if ((v2.z<0 || v2.z>1) && (v1.z<0 || v1.z>1)) continue;
                
                //v1.x/=v1.z;
                //v1.y/=v1.z;
                
                //v2.x/=v2.z;
                //v2.y/=v2.z;
                
                //std::cout << v1 << std::endl;
                
                v1.z = 0;
                v2.z = 0;
    
                Vector2 normal = { v2.y - v1.y, v1.x - v2.x };
                normal.Normalize();
                
//    Vector3 Position;
//    Vector2 TextureCoords;
//    Colour Color;
//    Vector3 Normal;

                Colorable* colorable = o->GetComponent<Colorable>();

                const Colour& color = colorable ? colorable->Color : Colour(1.0f, 1.0f, 1.0f, 1.0f);
                Colour alphaColor = color;
                alphaColor.A(0);

                short index = (short)v.size();
                float edgeWidth = width + 1.0f;
                
                
                
                v.push_back({ Vector3( v1 + normal * edgeWidth), Vector2(0,0), alphaColor, Vector3(0,0,1)});
                v.push_back({ Vector3( v1 + normal * width), Vector2(0,0), color, Vector3(0,0,1)});
                v.push_back({ Vector3( v1 - normal * width), Vector2(0,0), color, Vector3(0,0,1)});
                v.push_back({ Vector3( v1 - normal * edgeWidth), Vector2(0,0), alphaColor, Vector3(0,0,1)});
                
                v.push_back({ Vector3( v2 + normal * edgeWidth), Vector2(0,0), alphaColor, Vector3(0,0,1)});
                v.push_back({ Vector3( v2 + normal * width), Vector2(0,0), color, Vector3(0,0,1)});
                v.push_back({ Vector3( v2 - normal * width), Vector2(0,0), color, Vector3(0,0,1)});
                v.push_back({ Vector3( v2 - normal * edgeWidth), Vector2(0,0), alphaColor, Vector3(0,0,1)});
                
                for (int p=0; p<3; p++) {
    
                    t.emplace_back((short)index+p);
                    t.emplace_back((short)index+p+5);
                    t.emplace_back((short)index+p+1);
                    
                    t.emplace_back((short)index+p);
                    t.emplace_back((short)index+p+4);
                    t.emplace_back((short)index+p+5);
                    
                }
                                
            }
        }
    }
    
    
        
    world.Update(dt);
}

void LineRendererSystem::Render() {
    world.Render();
}
