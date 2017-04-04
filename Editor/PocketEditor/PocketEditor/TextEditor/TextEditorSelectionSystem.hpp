//
//  TextEditorCursorSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "TextEditor.hpp"
#include "TextBox.hpp"
#include "TextEditorRenderer.hpp"
#include "Sizeable.hpp"
#include "Font.hpp"
#include "VertexMesh.hpp"

using namespace Pocket;

class TextEditorSelectionSystem : public GameSystem<TextEditor, TextBox, TextEditorRenderer, Sizeable, Font> {
protected:
    void Initialize() override;
    void Destroy() override;
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void Update(float dt) override;
private:
    void UpdateMesh(GameObject* object);
    void AddQuad(VertexMesh<Vertex>& mesh, const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4);
};
