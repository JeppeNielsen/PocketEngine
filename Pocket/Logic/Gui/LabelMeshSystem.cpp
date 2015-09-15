//
//  SpriteMeshSystem.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "LabelMeshSystem.hpp"
#include "Atlas.hpp"
#include "Vertex.hpp"
#include "Colorable.hpp"

using namespace Pocket;

void LabelMeshSystem::Initialize() {
    AddComponent<Label>();
    AddComponent<Font>();
    AddComponent<Sizeable>();
    AddComponent<Mesh>();
}

void LabelMeshSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Sizeable>()->Size.Changed += event_handler(this, &LabelMeshSystem::SizeChanged, object);
    Label* label = object->GetComponent<Label>();
    label->FontSize.Changed += event_handler(this, &LabelMeshSystem::LabelChanged, object);
    label->Text.Changed += event_handler(this, &LabelMeshSystem::LabelChanged, object);
    label->HAlignment.Changed += event_handler(this, &LabelMeshSystem::LabelChanged, object);
    label->VAlignment.Changed += event_handler(this, &LabelMeshSystem::LabelChanged, object);
    label->WordWrap.Changed += event_handler(this, &LabelMeshSystem::LabelChanged, object);
    SomethingChanged(object);
}

void LabelMeshSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Sizeable>()->Size.Changed -= event_handler(this, &LabelMeshSystem::SizeChanged, object);
    Label* label = object->GetComponent<Label>();
    label->FontSize.Changed -= event_handler(this, &LabelMeshSystem::LabelChanged, object);
    label->Text.Changed -= event_handler(this, &LabelMeshSystem::LabelChanged, object);
    label->HAlignment.Changed -= event_handler(this, &LabelMeshSystem::LabelChanged, object);
    label->VAlignment.Changed -= event_handler(this, &LabelMeshSystem::LabelChanged, object);
    label->WordWrap.Changed -= event_handler(this, &LabelMeshSystem::LabelChanged, object);
}

void LabelMeshSystem::SizeChanged(Pocket::Sizeable *sizeable, GameObject* context) {
    SomethingChanged(context);
}

void LabelMeshSystem::LabelChanged(Pocket::Label *label, GameObject* context) {
    SomethingChanged(context);
}

void LabelMeshSystem::SomethingChanged(GameObject* object) {
    
    Sizeable* sizeable = object->GetComponent<Sizeable>();
    Mesh* mesh = object->GetComponent<Mesh>();
    Font* font = object->GetComponent<Font>();
    Label* label = object->GetComponent<Label>();
    Atlas* atlas = object->GetComponent<Atlas>();
    
    const Vector2& size = sizeable->Size.GetValue();
    
    const Box& texCoords = atlas ? atlas->GetNode(font->FontAtlasNode).outer : Box(0,0,1,1);
    
    mesh->Clear();
    
    Colour color = object->GetComponent<Colorable>()!=0 ? object->GetComponent<Colorable>()->Color() : Colour::White();
    AddText(*mesh, *font, label->Text, size, label->FontSize, label->HAlignment, label->VAlignment, label->WordWrap, texCoords, color);
}

 void LabelMeshSystem::AddText(Mesh& mesh, const Pocket::Font &font, std::string text, const Pocket::Vector2& size, float fontSize, Font::HAlignment hAlign, Font::VAlignment vAlign, bool wordWrap, const Box& t, const Colour& color) {
 
 std::vector<Font::Letter> letters;
 font.CreateText(letters, text, size, fontSize, hAlign, vAlign, wordWrap, true);
 
 float w = t.right - t.left;
 float h = t.bottom - t.top;
 
 for (int i=0; i<letters.size(); i++) {
     letters[i].u1 = t.left + letters[i].u1 * w;
     letters[i].u2 = t.left + letters[i].u2 * w;
     letters[i].v1 = t.top + letters[i].v1 * h;
     letters[i].v2 = t.top + letters[i].v2 * h;
 }
     
     
     auto& vertices = mesh.Vertices();
     auto& triangles = mesh.Triangles();
 
 size_t verticesIndex;
 
 verticesIndex = vertices.size();
 
 vertices.resize(vertices.size() + letters.size() * 4);
 
 Vector2 of = -0.5f / 1024.0f;
 
 
 for (int i=0; i<letters.size(); i++) {
 
 
 triangles.push_back((GLshort)(verticesIndex));
 triangles.push_back((GLshort)(verticesIndex+1));
 triangles.push_back((GLshort)(verticesIndex+2));
 
 triangles.push_back((GLshort)(verticesIndex));
 triangles.push_back((GLshort)(verticesIndex+2));
 triangles.push_back((GLshort)(verticesIndex+3));
 
 Vertex& v0 = vertices[verticesIndex + 0];
 Vertex& v1 = vertices[verticesIndex + 1];
 Vertex& v2 = vertices[verticesIndex + 2];
 Vertex& v3 = vertices[verticesIndex + 3];
 
 
 v0.Position = Vector2(letters[i].x, letters[i].y);
 v1.Position = Vector2(letters[i].x+letters[i].width, letters[i].y);
 v2.Position = Vector2(letters[i].x+letters[i].width, letters[i].y+letters[i].height);
 v3.Position = Vector2(letters[i].x, letters[i].y+letters[i].height);
 
 
 v0.TextureCoords.x = letters[i].u1 +of.x;
 v0.TextureCoords.y = letters[i].v2;
 
 v1.TextureCoords.x = letters[i].u2;
 v1.TextureCoords.y = letters[i].v2;
 
 v2.TextureCoords.x = letters[i].u2;
 v2.TextureCoords.y = letters[i].v1+ of.x;
 
 v3.TextureCoords.x = letters[i].u1+ of.x;
 v3.TextureCoords.y = letters[i].v1+ of.x;
 
 
 v0.Color = color;
 v1.Color = color;
 v2.Color = color;
 v3.Color = color;
 
 verticesIndex += 4;
 }

}
 




