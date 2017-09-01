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
    viewport->renderTexture.Initialize(viewport->RenderTextureSize.x, viewport->RenderTextureSize.y);
    object->GetComponent<TextureComponent>()->Texture().SetRenderTexture(&viewport->renderTexture, viewport->RenderTextureSize.x, viewport->RenderTextureSize.y);
    object->GetComponent<Touchable>()->Down.Bind(this, &GameWorldViewportSystem::TouchDown, object);
    object->GetComponent<Touchable>()->Up.Bind(this, &GameWorldViewportSystem::TouchUp, object);
    viewport->inputDevice.Initialize(12);
}

void GameWorldViewportSystem::ObjectRemoved(GameObject* object) {
    object->GetComponent<TextureComponent>()->Texture().SetRenderTexture(nullptr, 0,0);
    object->GetComponent<Touchable>()->Down.Unbind(this, &GameWorldViewportSystem::TouchDown, object);
    object->GetComponent<Touchable>()->Up.Unbind(this, &GameWorldViewportSystem::TouchUp, object);
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

void GameWorldViewportSystem::TouchDown(Pocket::TouchData d, Pocket::GameObject *object) {
    auto viewport = object->GetComponent<GameWorldViewport>();
    Vector2 local = object->GetComponent<Transform>()->WorldInverse().TransformPosition(d.WorldPosition);
    viewport->inputDevice.SetTouch(d.Index, true, local);
}

void GameWorldViewportSystem::TouchUp(Pocket::TouchData d, Pocket::GameObject *object) {
    auto viewport = object->GetComponent<GameWorldViewport>();
    Vector2 local = object->GetComponent<Transform>()->WorldInverse().TransformPosition(d.WorldPosition);
    viewport->inputDevice.SetTouch(d.Index, false, local);
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
    Vector2 uv = { size.x / (float)viewport->RenderTextureSize.x,
                   size.y / (float)viewport->RenderTextureSize.y};
    
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
    
    Vector2 oldScreenSize = Engine::Context().ScreenSize;
    float oldScalingFactor = Engine::Context().ScreenScalingFactor;
    Engine::Context().ScreenSize = size;
    Engine::Context().ScreenScalingFactor = 1.0f;
    
    const Matrix4x4& inv = object->GetComponent<Transform>()->WorldInverse();
    
    for (int i=0; i<12; i++) {
        Vector2 local = inv.TransformPosition(root->Input().GetDevice()->GetTouchPosition(i));
        viewport->inputDevice.SetTouchPosition(i, local);
    }
    
    viewport->inputDevice.StartFrame(nullptr);
    viewport->inputDevice.UpdateInputManager(&viewport->World->Input());
    viewport->World->Update(dt);
    viewport->inputDevice.EndFrame();
    
    Engine::Context().ScreenSize = oldScreenSize;
    Engine::Context().ScreenScalingFactor = oldScalingFactor;
}

void GameWorldViewportSystem::RenderObject(GameObject* object) {
    auto viewport = object->GetComponent<GameWorldViewport>();
    auto sizeable = object->GetComponent<Sizeable>();
    if (!viewport->World) return;
    
    viewport->renderTexture.Begin();
    glClearColor(0,0,0.7f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Vector2 oldScreenSize = Engine::Context().ScreenSize;
    float oldScalingFactor = Engine::Context().ScreenScalingFactor;
    Engine::Context().ScreenSize = sizeable->Size; //Vector2(viewport->RenderSize.x, viewport->RenderSize.y);
    Engine::Context().ScreenScalingFactor = 1.0f;
    viewport->World->Render();
    Engine::Context().ScreenSize = oldScreenSize;
    Engine::Context().ScreenScalingFactor = oldScalingFactor;
    
    viewport->renderTexture.End();
}
