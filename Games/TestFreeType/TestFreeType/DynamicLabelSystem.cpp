//
//  DynamicLabelSystem.cpp
//  TestFreeType
//
//  Created by Jeppe Nielsen on 13/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "DynamicLabelSystem.hpp"

using namespace Pocket;

void DynamicLabelSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<Sizeable>()->Size.Changed.Bind(this, &DynamicLabelSystem::SomethingChanged, object);
    DynamicLabel* label = object->GetComponent<DynamicLabel>();
    label->FontSize.Changed.Bind(this, &DynamicLabelSystem::TextChanged, object);
    label->Text.Changed.Bind(this, &DynamicLabelSystem::TextChanged, object);
    label->HAlignment.Changed.Bind(this, &DynamicLabelSystem::SomethingChanged, object);
    label->VAlignment.Changed.Bind(this, &DynamicLabelSystem::SomethingChanged, object);
    label->WordWrap.Changed.Bind(this, &DynamicLabelSystem::SomethingChanged, object);
    object->GetComponent<Font>()->BufferUpdated.Bind(this, &DynamicLabelSystem::SomethingChanged, object);
    
    TextChanged(object);
}

void DynamicLabelSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<Sizeable>()->Size.Changed.Unbind(this, &DynamicLabelSystem::SomethingChanged, object);
    DynamicLabel* label = object->GetComponent<DynamicLabel>();
    label->FontSize.Changed.Unbind(this, &DynamicLabelSystem::TextChanged, object);
    label->Text.Changed.Unbind(this, &DynamicLabelSystem::TextChanged, object);
    label->HAlignment.Changed.Unbind(this, &DynamicLabelSystem::SomethingChanged, object);
    label->VAlignment.Changed.Unbind(this, &DynamicLabelSystem::SomethingChanged, object);
    label->WordWrap.Changed.Unbind(this, &DynamicLabelSystem::SomethingChanged, object);
    object->GetComponent<Font>()->BufferUpdated.Unbind(this, &DynamicLabelSystem::SomethingChanged, object);
}

void DynamicLabelSystem::SomethingChanged(GameObject* object) {
    dirtyObjects.insert(object);
}

void DynamicLabelSystem::TextChanged(Pocket::GameObject *object) {
    DynamicLabel* label = object->GetComponent<DynamicLabel>();
    object->GetComponent<Font>()->RequestText(label->Text, label->FontSize);
    dirtyObjects.insert(object);
}

void DynamicLabelSystem::UpdateDirtyObject(Pocket::GameObject *object) {
    Sizeable* sizeable = object->GetComponent<Sizeable>();
    Mesh* mesh = object->GetComponent<Mesh>();
    Font* font = object->GetComponent<Font>();
    DynamicLabel* label = object->GetComponent<DynamicLabel>();
    const Vector2& size = sizeable->Size;
    mesh->Clear();
    Colour color = Colour::White();// object->GetComponent<Colorable>()!=0 ? object->GetComponent<Colorable>()->Color() : Colour::White();
    AddText(*mesh, *font, label->Text, size, label->FontSize, label->HAlignment, label->VAlignment, label->WordWrap, color);
}

 void DynamicLabelSystem::AddText(Mesh& mesh, const Font &font, std::string text, const Pocket::Vector2& size, float fontSize, Font::HAlignment hAlign, Font::VAlignment vAlign, bool wordWrap, const Colour& color) {
 
     std::vector<Font::Letter> letters;
     font.CreateText(letters, text, size, fontSize, hAlign, vAlign, wordWrap, true);
     
         
     auto& vertices = mesh.Vertices();
     auto& triangles = mesh.Triangles();
     
     size_t verticesIndex;
     
     verticesIndex = vertices.size();
     
     vertices.resize(vertices.size() + letters.size() * 4);
     
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
         
         v0.TextureCoords.x = letters[i].u1;
         v0.TextureCoords.y = letters[i].v2;
         
         v1.TextureCoords.x = letters[i].u2;
         v1.TextureCoords.y = letters[i].v2;
         
         v2.TextureCoords.x = letters[i].u2;
         v2.TextureCoords.y = letters[i].v1;
         
         v3.TextureCoords.x = letters[i].u1;
         v3.TextureCoords.y = letters[i].v1;
         
         v0.Color = color;
         v1.Color = color;
         v2.Color = color;
         v3.Color = color;
         
         verticesIndex += 4;
     }
}

void DynamicLabelSystem::Update(float dt) {
    for(auto o : dirtyObjects) {
        UpdateDirtyObject(o);
    }
    dirtyObjects.clear();
}

