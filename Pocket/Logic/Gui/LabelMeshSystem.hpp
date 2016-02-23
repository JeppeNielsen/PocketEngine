//
//  SpriteMeshSystem.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/1/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "GameSystem.hpp"
#include "Mesh.hpp"
#include "Sizeable.hpp"
#include "Label.hpp"
#include "Font.hpp"

namespace Pocket {
    template<typename T>
    class LabelMeshSystem : public GameSystem<T, Label, Font, Mesh, Sizeable> {
    public:
        using GameObject = GameObject<T>;
    
        void ObjectAdded(GameObject *object) {
            object->template GetComponent<Sizeable>()->Size.Changed.Bind(this, &LabelMeshSystem::SizeChanged, object);
            Label* label = object->template GetComponent<Label>();
            /*label->FontSize.Changed.Bind(this, &LabelChanged, object);
            label->Text.Changed.Bind(this, &LabelChanged, object);
            label->HAlignment.Changed.Bind(this, &LabelChanged, object);
            label->VAlignment.Changed.Bind(this, &LabelChanged, object);
            label->WordWrap.Changed.Bind(this, &LabelChanged, object);
            */
            
            SomethingChanged(object);
        }

        void ObjectRemoved(GameObject *object) {
            object->template GetComponent<Sizeable>()->Size.Changed.Unbind(this, &LabelMeshSystem::SizeChanged, object);
            /*
            Label* label = object->GetComponent<Label>();
            label->FontSize.Changed.Unbind(this, &LabelChanged, object);
            label->Text.Changed.Unbind(this, &LabelChanged, object);
            label->HAlignment.Changed.Unbind(this, &LabelChanged, object);
            label->VAlignment.Changed.Unbind(this, &LabelChanged, object);
            label->WordWrap.Changed.Unbind(this, &LabelChanged, object);
            */
        }

        void SizeChanged(Vector2& size, GameObject* context) {
            SomethingChanged(context);
        }

        void LabelChanged(Pocket::Label *label, GameObject* context) {
            SomethingChanged(context);
        }

        void SomethingChanged(GameObject* object) {
            
            Sizeable* sizeable = object->template GetComponent<Sizeable>();
            Mesh* mesh = object->template GetComponent<Mesh>();
            Font* font = object->template GetComponent<Font>();
            Label* label = object->template GetComponent<Label>();
            Atlas* atlas = object->template GetComponent<Atlas>();
            
            const Vector2& size = sizeable->Size;
            
            const Box& texCoords = atlas ? atlas->GetNode(font->FontAtlasNode).outer : Box(0,0,1,1);
            
            mesh->Clear();
            
            Colour color = Colour::White();// object->GetComponent<Colorable>()!=0 ? object->GetComponent<Colorable>()->Color() : Colour::White();
            AddText(*mesh, *font, label->Text, size, label->FontSize, label->HAlignment, label->VAlignment, label->WordWrap, texCoords, color);
        }

         void AddText(Mesh& mesh, const Pocket::Font &font, std::string text, const Pocket::Vector2& size, float fontSize, Font::HAlignment hAlign, Font::VAlignment vAlign, bool wordWrap, const Box& t, const Colour& color) {
         
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
    };
}