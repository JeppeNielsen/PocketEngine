//
//  GameObjectEditorSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GameObjectEditorSystem.hpp"
#include "FieldEditor.hpp"
using namespace Pocket;

GameObjectEditorSystem::GameObjectEditorSystem()
#ifdef SCRIPTING_ENABLED
: scriptWorld(0)
#endif
{ Predicate = 0; }

void GameObjectEditorSystem::Initialize() {
}

void GameObjectEditorSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<GameObjectEditor>()->Object.Changed.Bind(this, &GameObjectEditorSystem::ObjectChanged, object);
    dirtyObjects.insert(object);
}

void GameObjectEditorSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<GameObjectEditor>()->Object.Changed.Unbind(this, &GameObjectEditorSystem::ObjectChanged, object);
    GameObjectEditor *editor = object->GetComponent<GameObjectEditor>();
    for(auto e : editor->editors) {
        e.editor->Destroy();
        delete e.editor;
    }
    editor->editors.clear();
    
    auto it = dirtyObjects.find(object);
    if (it!=dirtyObjects.end()) {
        dirtyObjects.erase(object);
    }
}

void GameObjectEditorSystem::Update(float dt) {
    for(auto dirtyObject : dirtyObjects) {
        CreateEditors(dirtyObject);
    }
    dirtyObjects.clear();
    for(auto o : Objects()) {
        GameObjectEditor* editor = o->GetComponent<GameObjectEditor>();
        for(auto editor : editor->editors) {
            editor.editor->Update(dt);
        }
    }
}

void GameObjectEditorSystem::ObjectChanged(GameObject* object) {
    dirtyObjects.insert(object);
}

void GameObjectEditorSystem::CreateEditors(GameObject* object) {
    GameObjectEditor *editor = object->GetComponent<GameObjectEditor>();
    
    for(auto e : editor->editors) {
        e.editor->Destroy();
        delete e.editor;
    }
    editor->editors.clear();
    
    for (auto child : object->Hierarchy().Children()) {
        child->Remove();
    }
    if (!editor->Object()) return;
    
    Vector2 size = { 200, 100 };
    
    GameObject* control = gui->CreateControl(object, "Box", 0, size);
    gui->AddLayouter(control, {40,50}, 2000, 2000);
    control->RemoveComponent<Touchable>();
    
    editor->editors = editor->Object()->GetComponentEditors(Predicate);

    std::vector<GameObject*> controls;

    for(auto componentEditor : editor->editors) {
        int countBefore = (int)control->Hierarchy().Children().size();
        componentEditor.editor->Create(componentEditor.type.name, gui, control, editor->Object());
        if (control->Hierarchy().Children().size()>countBefore) {
            GameObject* editorControl = control->Hierarchy().Children()[control->Hierarchy().Children().size()-1];
            controls.push_back(editorControl);
            if (!editorControl->GetComponent<Touchable>()) {
                editorControl->AddComponent<Touchable>();
            }
        }
    }
    
    for (int i=0; i<controls.size(); ++i) {
        if (controls[i]) {
            editor->ComponentEditorCreated({ editor->editors[i].type, controls[i]});
        }
    }
}
