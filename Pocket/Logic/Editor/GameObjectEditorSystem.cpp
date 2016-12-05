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
{}

void GameObjectEditorSystem::Initialize() {
}

void GameObjectEditorSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<GameObjectEditor>()->Object.Changed.Bind(this, &GameObjectEditorSystem::ObjectChanged, object);
    ObjectChanged(object);
}

void GameObjectEditorSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<GameObjectEditor>()->Object.Changed.Unbind(this, &GameObjectEditorSystem::ObjectChanged, object);
    GameObjectEditor *editor = object->GetComponent<GameObjectEditor>();
    for(auto e : editor->editors) {
        e->Destroy();
        delete e;
    }
    editor->editors.clear();
}

void GameObjectEditorSystem::Update(float dt) {
    for(auto o : Objects()) {
        GameObjectEditor* editor = o->GetComponent<GameObjectEditor>();
        for(auto editor : editor->editors) {
            editor->Update(dt);
        }
    }
}

void GameObjectEditorSystem::ObjectChanged(GameObject* object) {
    GameObjectEditor *editor = object->GetComponent<GameObjectEditor>();
    
    for(auto e : editor->editors) {
        e->Destroy();
        delete e;
    }
    editor->editors.clear();
    
    for (auto child : object->Children()) {
        child->Remove();
    }
    if (!editor->Object()) return;
    
    Vector2 size = { 200, 100 };
    
    
    GameObject* control = gui->CreateControl(object, "Box", 0, size);
    auto layouter = control->AddComponent<Layouter>();
    layouter->ChildrenLayoutMode = Layouter::LayoutMode::Vertical;
    
    
    
    editor->editors = editor->Object()->GetComponentEditors();

    for(auto editor : editor->editors) {
        editor->Create(gui, control);
    }
    
    return;
    
    auto infos = editor->Object()->GetComponentTypes([this] (int componentID) {
        return std::find(ignoredComponents.begin(), ignoredComponents.end(), componentID)==ignoredComponents.end();
    });
    /*
    
#ifdef SCRIPTING_ENABLED
    if (scriptWorld) {
        int numberOfScriptComponents = scriptWorld->ComponentCount();
        
        for(int i=0; i<numberOfScriptComponents; ++i) {
            TypeInfo info = scriptWorld->GetTypeInfo(*editor->Object(), i);
            if (!info.fields.empty()) {
                infos.push_back(info);
            }
        }
    }
#endif
    */
    
    
    
    int counter = 0;
    for (auto info : infos) {
        std::cout << "Field : " << info.name << std::endl;
    
        /*bool hasNoEditors = true;
        for (auto field : info.fields) {
            if (field->HasEditor()) {
                hasNoEditors = false;
                break;
            }
        }
        if (hasNoEditors) {
            //continue;
        }
        */
        
        GameObject* componentChild = gui->CreateControl(object, "Box", {0, counter*size.y}, size);
        //componentChild->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
        //componentChild->GetComponent<Layoutable>()->ChildLayouting = Layoutable::ChildLayouting::VerticalStackedFit;
        
        GameObject* space = componentChild->CreateChild();
        space->AddComponent<Transform>();
        space->AddComponent<Sizeable>()->Size = {size.x, 10};
        //space->AddComponent<Layoutable>();
        for (auto field : info.fields) {
            //if (!field->HasEditor()) continue;
            
            GameObject* editor = componentChild->CreateChild();
            editor->AddComponent<Transform>();
            editor->AddComponent<Sizeable>()->Size = {size.x, size.y/(info.fields.size()+1)};
            //editor->AddComponent<Layoutable>();
            FieldEditor* fieldEditor = editor->AddComponent<FieldEditor>();
            fieldEditor->SetType(info);
            fieldEditor->Field = field->name;
        }
        
        GameObject* componentName = gui->CreateLabel(componentChild, 0, {size.x, 20}, 0, info.name, 14);
        componentName->GetComponent<Label>()->HAlignment = Font::Center;
        componentName->GetComponent<Label>()->VAlignment = Font::Middle;
        componentName->GetComponent<Colorable>()->Color = Colour::Black();
        //componentName->AddComponent<Layoutable>();
        
        editor->ComponentEditorCreated({ info, componentChild });
        counter++;
    }
}