//
//  TextEditorCursorSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "TextEditorSelectionMeshSystem.hpp"
#include "Transform.hpp"
#include "Renderable.hpp"
#include "Mesh.hpp"
#include "MathHelper.hpp"
#include "Orderable.hpp"

void TextEditorSelectionMeshSystem::Initialize() {

}

void TextEditorSelectionMeshSystem::Destroy() {
    
}

void TextEditorSelectionMeshSystem::ObjectAdded(Pocket::GameObject *object) {
    GameObject* selection = object->CreateChild();
    selection->AddComponent<Transform>();
    selection->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    selection->AddComponent<Mesh>();
    selection->AddComponent<Orderable>();
    SetMetaData(object, selection);
}

void TextEditorSelectionMeshSystem::ObjectRemoved(Pocket::GameObject *object) {
    GameObject* selection = static_cast<GameObject*>(GetMetaData(object));
    selection->Remove();
}

void TextEditorSelectionMeshSystem::Update(float dt) {
    for(auto o : Objects()) {
        UpdateMesh(o);
    }
}

void TextEditorSelectionMeshSystem::AddQuad(VertexMesh<Pocket::Vertex> &mesh, const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4) {
    size_t size = mesh.vertices.size();
    
    mesh.vertices.push_back({ {v1.x, v1.y,0 }, {0,0}, Colour(1.0f, 1.0f, 1.0f, 0.4f), {0,0,1} });
    mesh.vertices.push_back({ {v2.x, v2.y,0 }, {0,0}, Colour(1.0f, 1.0f, 1.0f, 0.4f), {0,0,1} });
    mesh.vertices.push_back({ {v3.x, v3.y,0 }, {0,0}, Colour(1.0f, 1.0f, 1.0f, 0.4f), {0,0,1} });
    mesh.vertices.push_back({ {v4.x, v4.y,0 }, {0,0}, Colour(1.0f, 1.0f, 1.0f, 0.4f), {0,0,1} });
    
    mesh.triangles.push_back(size + 0);
    mesh.triangles.push_back(size + 2);
    mesh.triangles.push_back(size + 1);
    mesh.triangles.push_back(size + 0);
    mesh.triangles.push_back(size + 3);
    mesh.triangles.push_back(size + 2);
}

void TextEditorSelectionMeshSystem::UpdateMesh(Pocket::GameObject *object) {
    TextEditor* textEditor = object->GetComponent<TextEditor>();
    
    GameObject* selection = static_cast<GameObject*>(GetMetaData(object));
    
    auto& mesh = selection->GetComponent<Mesh>()->GetMesh<Vertex>();
    
    auto& v = mesh.vertices;
    auto& t = mesh.triangles;
    
    v.clear();
    t.clear();
    
    if (!textEditor->SelectionActive) return;
    
    int min;
    int max;
    if (textEditor->Selection().x<textEditor->Selection().y) {
        min = textEditor->Selection().x;
        max = textEditor->Selection().y;
    } else {
        min = textEditor->Selection().y;
        max = textEditor->Selection().x;
    }
    
    int selectionSize = max - min;
    if (selectionSize == 0) return;
    
    Point start = textEditor->CursorToCartesian(min);
    Point end = textEditor->CursorToCartesian(max);
    
    
    TextEditorRenderer* renderer = object->GetComponent<TextEditorRenderer>();
    Font* font = object->GetComponent<Font>();
    Vector2 size = object->GetComponent<Sizeable>()->Size;
    
    float fontSize = renderer->fontSize;
    float spacing = font->GetSpacing(fontSize);
    
    Vector2 scaler = { spacing, renderer->fontSize };
    
    /*
    Vector3 Position;
    Vector2 TextureCoords;
    Colour Color;
    Vector3 Normal;
    */
    
    //v.push_back({ {0,0,0}, {0,0}, Colour(1.0f, 1.0f, 1.0f, 1.0f), {0,0,1} });
    
    Vector2 sp = {
        -renderer->Offset().x + start.x * scaler.x,
        renderer->Offset().y + size.y - fontSize - start.y * scaler.y
    };
    
    Vector2 ep = {
        -renderer->Offset().x + end.x * scaler.x,
        renderer->Offset().y + size.y - fontSize - end.y * scaler.y
    };
    
    sp.x = MathHelper::Clamp(sp.x, 0, size.x);
    sp.y = MathHelper::Clamp(sp.y, 0, size.y);
    
    ep.x = MathHelper::Clamp(ep.x, 0, size.x);
    ep.y = MathHelper::Clamp(ep.y, 0, size.y);
    
    if (start.y == end.y) {
        AddQuad(mesh,
            { sp.x, sp.y + fontSize }, { ep.x, sp.y + fontSize },
            { ep.x, ep.y }, { sp.x, sp.y }
        );
    } else {
        AddQuad(mesh,
            { sp.x, sp.y + fontSize }, { size.x, sp.y + fontSize },
            { size.x, sp.y }, { sp.x, sp.y }
        );
        
        int height = (end.y - start.y) - 1;
        if (height>0) {
            AddQuad(mesh,
                { 0, sp.y }, { size.x, sp.y },
                { size.x, ep.y + fontSize }, { 0, ep.y + fontSize }
            );
        }
        
        
        AddQuad(mesh,
            { 0, ep.y + fontSize }, { ep.x, ep.y + fontSize },
            { ep.x, ep.y }, { 0, ep.y }
        );
    
    }
}
