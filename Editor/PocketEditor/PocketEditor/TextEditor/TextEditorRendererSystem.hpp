//
//  TextEditorRendererSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "TextEditor.hpp"
#include "TextEditorRenderer.hpp"
#include "Font.hpp"
#include "Sizeable.hpp"
#include "Mesh.hpp"
#include <set>

using namespace Pocket;

struct TextEditorRendererSystem : GameSystem<TextEditor, TextEditorRenderer, Font, Sizeable, Mesh> {
    void Initialize() override;
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    void Update(float dt) override;
    
    void ObjectChanged(GameObject* object);
    
    std::set<GameObject*> dirtyObjects;
    
    void UpdateMesh(GameObject* object);
};
