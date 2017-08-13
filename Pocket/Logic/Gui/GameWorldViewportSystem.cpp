//
//  GameWorldViewportSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 13/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "GameWorldViewportSystem.hpp"
#include "Engine.hpp"

using namespace Pocket;

void GameWorldViewportSystem::ObjectAdded(GameObject* object) {
    auto viewport = object->GetComponent<GameWorldViewport>();
    viewport->renderTexture.Initialize(viewport->RenderSize.x, viewport->RenderSize.y);
    object->GetComponent<TextureComponent>()->Texture().SetCustomTexture(viewport->renderTexture.GetTexture(), viewport->RenderSize.x, viewport->RenderSize.y);
}

void GameWorldViewportSystem::Update(float dt) {
    for(auto o : Objects()) {
        UpdateObject(o, dt);
    }
}

void GameWorldViewportSystem::Render() {
    for(auto o : Objects()) {
        RenderObject(o);
    }
}

void GameWorldViewportSystem::UpdateObject(GameObject* object, float dt) {
    auto viewport = object->GetComponent<GameWorldViewport>();
    if (!viewport->World) return;
    
    Sizeable* sizeable = object->GetComponent<Sizeable>();
    auto& mesh = object->GetComponent<Mesh>()->GetMesh<Vertex>();
    
    if (mesh.vertices.size()!=4) {
        mesh.vertices.resize(4);
    }
    if (mesh.triangles.size()!=6) {
        mesh.triangles.resize(6);
        
        mesh.triangles[0]=0;
        mesh.triangles[1]=1;
        mesh.triangles[2]=2;
        
        mesh.triangles[3]=0;
        mesh.triangles[4]=2;
        mesh.triangles[5]=3;
    }
    
    Vector2 size = sizeable->Size;
    Vector2 uv = 1;
    
    mesh.vertices[0].Position = {0,0,0};
    mesh.vertices[1].Position = {size.x,0,0};
    mesh.vertices[2].Position = {size.x,size.y,0};
    mesh.vertices[3].Position = {0,size.y,0};
    
    for(int i=0; i<4; ++i) {
        mesh.vertices[i].Color = Colour::White();
    }
    
    mesh.vertices[0].TextureCoords = {0,0};
    mesh.vertices[1].TextureCoords = {uv.x,0};
    mesh.vertices[2].TextureCoords = uv;
    mesh.vertices[3].TextureCoords = {0,uv.y};
    
    Vector2 worldPosition = object->GetComponent<Transform>()->World().TransformPosition(0);
    
    Vector2 scale = { size.x / viewport->RenderSize.x, size.y / viewport->RenderSize.y };
    
    Matrix4x4 mat = Matrix4x4::CreateTranslation(worldPosition) * Matrix4x4::CreateScale({scale.x, scale.y,1});
    mat = mat.Invert();
    
    Vector2 oldScreenSize = Engine::Context().ScreenSize;
    float oldScalingFactor = Engine::Context().ScreenScalingFactor;
    Engine::Context().ScreenSize = Vector2(viewport->RenderSize.x, viewport->RenderSize.y);
    Engine::Context().ScreenScalingFactor = 1.0f;
    
    viewport->World->Input().SetTransformationMatrix(mat);
    root->Input().GetDevice()->UpdateInputManager(&viewport->World->Input());
   
    viewport->World->Update(dt);
    Engine::Context().ScreenSize = oldScreenSize;
    Engine::Context().ScreenScalingFactor = oldScalingFactor;
}

void GameWorldViewportSystem::RenderObject(GameObject* object) {
    auto viewport = object->GetComponent<GameWorldViewport>();
    if (!viewport->World) return;
    
    viewport->renderTexture.Begin();
    glClearColor(1,0,0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Vector2 oldScreenSize = Engine::Context().ScreenSize;
    float oldScalingFactor = Engine::Context().ScreenScalingFactor;
    Engine::Context().ScreenSize = Vector2(viewport->RenderSize.x, viewport->RenderSize.y);
    Engine::Context().ScreenScalingFactor = 1.0f;
    viewport->World->Render();
    Engine::Context().ScreenSize = oldScreenSize;
    Engine::Context().ScreenScalingFactor = oldScalingFactor;
    
    viewport->renderTexture.End();
}
