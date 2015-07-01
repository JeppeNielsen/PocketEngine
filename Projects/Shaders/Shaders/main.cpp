//
//  TestAutomaticVertex.cpp
//  Shaders
//
//  Created by Jeppe Nielsen on 21/06/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//
#include "Engine.hpp"
#include "Shader.h"
#include "MeshComponent.h"
#include "GameWorld.hpp"
#include "RenderSystem.h"
#include "Material.h"
#include "Timer.hpp"
#include <memory>

Component(Rotator)
    float speed;
    void Reset() { speed = 1.0f; }
};

SYSTEM(RotatorSystem, Rotator, Transform)
    void Update(float dt) {
        for(auto o : Objects()) {
            float speed = o->GetComponent<Rotator>()->speed * dt;
            Quaternion rot = o->GetComponent<Transform>()->Rotation;
            rot.Normalize();
            o->GetComponent<Transform>()->Rotation = rot * Quaternion(speed, {0.3f,1,0});
        }
    }
};

using namespace Pocket;

class TestShader : public GameState<TestShader> {

    GameWorld world;
    Shader<Vertex> colorShader;
    Shader<Vertex> blueShader;
    Timer timer;
    
    Transform* blueQuad;
    GameObject* camera;
    
    void InitializeShaders() {
         std::string vertexShader =
            "attribute vec4 Position;\n"
            "attribute vec2 TexCoords; \n"
            "attribute vec4 Color; \n"
            "attribute vec3 Normal; \n"
            "uniform mat4 ViewProjection;\n"
            "varying vec2 DestinationTexCoords;\n"
            "varying vec4 DestinationColor;\n"
            "varying vec3 DestinationNormal;\n"
            "void main(void) {\n"
            "	DestinationTexCoords = TexCoords;\n"
            "   DestinationColor = Color; \n"
            "   DestinationNormal = Normal; \n"
            "	gl_Position = Position * ViewProjection;\n"
            "}\n";
        
        std::string fragmentShader =
            "varying vec2 DestinationTexCoords;"
            "varying vec4 DestinationColor;"
            "varying vec3 DestinationNormal;"
            "uniform vec3 LightDirection;"
            "uniform vec4 AmbientLight;"
            "void main(void) {"
            "   float n = clamp(dot(LightDirection, DestinationNormal),0.0,1.0); "
            "	gl_FragColor = vec4(AmbientLight + DestinationColor * n);\n"  //texture2D(Texture, DestinationTexCoords) * DestinationColor;"
            "}";
        
        if (!colorShader.Initialize(vertexShader,fragmentShader)) {
            std::cout<< " Shader failed " << std::endl;
        }
        
        
        std::string fragmentShaderBlue =
            "varying vec2 DestinationTexCoords;"
            "varying vec4 DestinationColor;"
            "void main(void) {"
            "	gl_FragColor = vec4(0,0,1,0.5);" //texture2D(Texture, DestinationTexCoords) * DestinationColor;"
            "}";
        
        if (!colorShader.Initialize(vertexShader,fragmentShader)) {
            std::cout<< " Shader failed " << std::endl;
        }
        
        if (!blueShader.Initialize(vertexShader, fragmentShaderBlue)) {
            std::cout<< " Shader failed " << std::endl;
        }
        
        colorShader.name = "Color Shader";
        blueShader.name = "Blue Shader";
        
        
        colorShader.SetValue("LightDirection", Vector3(1,1,1).Normalized());
        colorShader.SetValue("AmbientLight", Colour(0.0f,0.0f,0.3f,1.0f));
    }
    
    void Initialize() {

        InitializeShaders();
    
        world.CreateSystem<RenderSystem>();
        world.CreateSystem<RotatorSystem>();
        
        camera = world.CreateObject();
        camera->AddComponent<Transform>()->Position = {0,0,5};
        camera->AddComponent<Camera>()->Viewport = Manager().Viewport();
        
        for (int i=0; i<2; i++) {
            auto o = world.CreateObject();
            o->AddComponent<Transform>()->Position = {0,0,i*-2.0f};
            o->AddComponent<Material>()->Shader = &colorShader;// i%2==0 ? &colorShader : &blueShader;
            o->GetComponent<Material>()->BlendMode = i==1 ? BlendMode::Opaque : BlendMode::Opaque;
            
            auto& mesh = o->AddComponent<MeshComponent>()->Mesh<Vertex>();
            
            if (i==1) {
                mesh.AddCube(0, {1,2,1});
            } else {
                mesh.AddGeoSphere(0, 1, 12);
                mesh.SetColor(Colour(1.0f,0.0f,0.0f,1.0f));
            }
            
            
            /*
            verts.push_back({ {-1,-1,0 } ,0,0, {1.0f,1.0f,0.0f }});
            verts.push_back({ {1,-1,0},0,0,{1.0f,0,0}});
            verts.push_back({ {1,1,0} });
            verts.push_back({ {-1,1+i*0.2f,0} });
            auto& triangles = o->GetComponent<MeshComponent>()->Triangles();
            triangles.push_back(0);
            triangles.push_back(1);
            triangles.push_back(2);
            
            triangles.push_back(0);
            triangles.push_back(2);
            triangles.push_back(3);
            
            triangles.push_back(0);
            triangles.push_back(2);
            triangles.push_back(1);
            
            triangles.push_back(0);
            triangles.push_back(3);
            triangles.push_back(2);
            */
            
            o->AddComponent<Rotator>()->speed = -1 + i* 0.2f;
            
            if (i==0) {
                blueQuad = o->GetComponent<Transform>();
            }
       }
        
        world.Update(0);
    }
    
    void Update(float dt) {
        //static float angle = 0;
        //angle+=dt;
        //camera->GetComponent<Transform>()->Rotation = Quaternion(angle, {0,1,0});
        
        blueQuad->Position = {0,0,2-Input.GetTouchPosition(0).x*0.01f };
    
        //shader.SetValue<float>(Input.GetTouchPosition(0).x*0.001f);
    
        world.Update(dt);
    }
    
    void Render() {
        
    
        timer.Begin();
        world.Render();
        double time = timer.End();
        time = 1.0/time;
        std::cout<<"Render Time = "<<time <<std::endl;
    }
    
    
};



int main() {
    Engine e;
    e.Start<TestShader>();
    return 0;
}