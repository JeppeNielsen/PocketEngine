//
//  GameMenu.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/09/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "GameObjectMenu.hpp"
#include "Timer.hpp"
#include "ShaderComponent.hpp"

std::string GameObjectMenu::Name() {
    return "GameObject";
}

void GameObjectMenu::OnInitialize() {

}

void GameObjectMenu::OnCreate() {
    
    menu->AddChild("New", "N").Clicked.Bind([this] {
        auto object = context->Project().Worlds.ActiveWorld()->Root()->CreateObject();
        object->AddComponent<Transform>()->Position = {0,0,0};
        object->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(0, 1);
        object->AddComponent<Renderable>();
        object->AddComponent<EditorObject>();
        
        auto& shader = object->AddComponent<ShaderComponent>()->GetShader<Vertex>();
        
        shader.Load(
            "attribute vec4 Position;                   "
            "attribute vec2 TexCoords;                  "
            "attribute vec4 Color;                      "
            "attribute vec3 Normal;                     "
            "uniform mat4 ViewProjection;               "
            "varying vec2 vTexCoords;                   "
            "varying vec4 vColor;                       "
            "varying vec3 vNormal;                      "
            "void main() {                              "
            "	vTexCoords = TexCoords;                 "
            "   vColor = Color;                         "
            "   vNormal = Normal;                       "
            "	gl_Position = Position * ViewProjection;"
            "}                                          "
            ,
            "varying vec2 vTexCoords;                   "
            "varying vec4 vColor;                       "
            "varying vec3 vNormal;                      "
            "uniform vec3 LightDirection;               "
            "uniform vec4 AmbientLight;                 "
            "uniform sampler2D Texture;                 "
            "void main() {                              "
            "   float n = clamp(dot(LightDirection, vNormal),0.0,1.0);   "
            "   vec4 textureColor = texture2D(Texture, vTexCoords);      "
            "   vec4 color =  textureColor * (AmbientLight + vColor * n);             "
            "	gl_FragColor = vec4(color.r, color.g, color.b, textureColor.a * vColor.a);"
            "}"
        );
        shader.SetValue("LightDirection", Vector3(1,1,1).Normalized());
        shader.SetValue("AmbientLight", Colour(0.2f));
        
        object->AddComponent<TextureComponent>()->Texture().LoadFromPng("/Projects/PocketEngine/Editor/Pong//Gfx/pipe.png");
        
    });
    menu->AddChild("Delete", "D").Clicked.Bind([this] {
        selectables = context->Project().GetSelectables();
        for(auto o : selectables->Selected()) {
            o->GetComponent<EditorObject>()->gameObject->Remove();
        }
    });
    menu->AddChild("Clone", "C").Clicked.Bind([this] {
        selectables = context->Project().GetSelectables();
        for(auto o : selectables->Selected()) {
            GameObject* source = o->GetComponent<EditorObject>()->gameObject;
            if (source->IsRoot()) continue;
            source->CreateCopy();
         }
    });
}
