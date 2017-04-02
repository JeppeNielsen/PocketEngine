//
//  TextEditorRendererSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TextEditorRendererSystem.hpp"

void TextEditorRendererSystem::Initialize() {

}

void TextEditorRendererSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<TextEditor>()->Lines.HasBecomeDirty.Bind(this, &TextEditorRendererSystem::ObjectChanged, object);
    object->GetComponent<Sizeable>()->Size.Changed.Bind(this, &TextEditorRendererSystem::ObjectChanged, object);
    ObjectChanged(object);
}

void TextEditorRendererSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<TextEditor>()->Lines.HasBecomeDirty.Unbind(this, &TextEditorRendererSystem::ObjectChanged, object);
    object->GetComponent<Sizeable>()->Size.Changed.Unbind(this, &TextEditorRendererSystem::ObjectChanged, object);
}

void TextEditorRendererSystem::Update(float dt) {
for(auto o : Objects()) {
    UpdateMesh(o);
}
/*
    for(auto o : dirtyObjects) {
        UpdateMesh(o);
    }
    dirtyObjects.clear();
*/
}

void TextEditorRendererSystem::ObjectChanged(GameObject* object) {
    dirtyObjects.insert(object);
}

void TextEditorRendererSystem::UpdateMesh(Pocket::GameObject *object) {
    Mesh* mesh = object->GetComponent<Mesh>();
    mesh->Clear();
    
    TextEditorRenderer* textEditorRenderer = object->GetComponent<TextEditorRenderer>();
    
    Font* font = object->GetComponent<Font>();
    
    
    Vector2 size = object->GetComponent<Sizeable>()->Size;
    float fontSize = textEditorRenderer->fontSize;
    
    float spacing = font->GetSpacing(fontSize);
    
    int numX = (int)ceil(size.x / spacing)+1;
    int numY = (int)ceil(size.y / fontSize);
    if (numX<=0) return;
    if (numY<=0) return;
    
    TextEditor* textEditor = object->GetComponent<TextEditor>();
    
    const std::string& text = textEditor->text;
    auto& lines = textEditor->Lines();
    
    auto& vertices = mesh->Vertices();
    auto& triangles = mesh->Triangles();
    
    Colour color = Colour::Black();
    
    int yPos = (int)floorf(textEditorRenderer->Offset().y / fontSize);
    int xPos = (int)floorf(textEditorRenderer->Offset().x / spacing);
    
    float subPosition = textEditorRenderer->Offset().y - yPos * fontSize;
    float subPositionX = textEditorRenderer->Offset().x - xPos * spacing;
    
    if (xPos<0) {
        numX += xPos;
    }
    
    for (int yLine=0; yLine<numY; ++yLine) {
        
        int lineNo = yPos + yLine;
        if (lineNo<0) continue;
        if (lineNo>=lines.size()) return;
    
        auto& line = lines[lineNo];
        int lineStart = (int)line.start + ((xPos>=0) ? xPos : 0);
        int lineWidth = (int)(line.end - lineStart);
        if (lineWidth <= 0) continue;
        if (lineWidth > numX) {
            lineWidth = numX;
        }
        std::string lineString = text.substr(lineStart, lineWidth);
        font->RequestText(lineString, fontSize);
        std::vector<Font::Letter> letters;
        font->CreateText(letters, lineString, 0, fontSize, Pocket::Font::HAlignment::Left, Font::VAlignment::Top, false, true);
        
        //letters.clear();
        //letters.push_back({0,0,size.x/(lineNo+1.0f), fontSize, 0,0,0,0} );
        
     size_t verticesIndex = vertices.size();
     
     vertices.resize(vertices.size() + letters.size() * 4);
     
     float yPos = subPosition + size.y - fontSize - fontSize*yLine; //subPosition + (numY - yLine) * fontSize;
     float posX = -subPositionX + (xPos<0 ? -xPos * spacing : 0);
     
     Vector2 subPixel = 0;//0.5f;
     
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
         
         v0.Position = Vector2(posX+subPixel.x+letters[i].x, subPixel.y+yPos+letters[i].y);
         v1.Position = Vector2(posX+subPixel.x+letters[i].x+letters[i].width, subPixel.y+yPos+letters[i].y);
         v2.Position = Vector2(posX+subPixel.x+letters[i].x+letters[i].width, subPixel.y+yPos+letters[i].y+letters[i].height);
         v3.Position = Vector2(posX+subPixel.x+letters[i].x, subPixel.y+yPos+letters[i].y+letters[i].height);
         
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
}
