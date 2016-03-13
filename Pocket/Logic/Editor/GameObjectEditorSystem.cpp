//
//  GameObjectEditorSystem.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 24/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "GameObjectEditorSystem.hpp"
#include "FieldEditor.hpp"
#include "Layoutable.hpp"
using namespace Pocket;

void GameObjectEditorSystem::Initialize(GameWorld *world) {
    this->world = world;
}

void GameObjectEditorSystem::ObjectAdded(GameObject *object) {
    object->GetComponent<GameObjectEditor>()->Object.Changed.Bind(this, &GameObjectEditorSystem::ObjectChanged, object);
    ObjectChanged(object);
}

void GameObjectEditorSystem::ObjectRemoved(GameObject *object) {
    object->GetComponent<GameObjectEditor>()->Object.Changed.Unbind(this, &GameObjectEditorSystem::ObjectChanged, object);
}

void GameObjectEditorSystem::ObjectChanged(GameObject* object) {
    GameObjectEditor *editor = object->GetComponent<GameObjectEditor>();
    for (auto child : object->Children()) {
        child->Remove();
    }
    if (!editor->Object()) return;
    
    /*
    Vector2 size = { 200, 50 };
    for (int i=0; i<world->components.size(); ++i) {
        auto isIgnoredComponent = std::find(ignoredComponents.begin(), ignoredComponents.end(), componentTypes[i]);
        if (isIgnoredComponent!=ignoredComponents.end()) {
            continue;
        }
        
        TypeInfo* component = components[i];
    
        bool hasNoEditors = true;
        auto fields = component->GetFields();
        for (auto field : fields.fields) {
            if (field->HasEditor()) {
                hasNoEditors = false;
                break;
            }
        }
        if (hasNoEditors) {
            continue;
        }
    
        GameObject* componentChild = world->CreateObject();
        componentChild->Parent = object;
        componentChild->AddComponent<Transform>();
        componentChild->AddComponent<Sizeable>()->Size = size;
        componentChild->AddComponent<Layoutable>()->HorizontalAlignment = Layoutable::HAlignment::Relative;
        componentChild->GetComponent<Layoutable>()->ChildLayouting = Layoutable::ChildLayouting::VerticalStackedFit;
        
        GameObject* space = world->CreateObject();
        space->Parent = componentChild;
        space->AddComponent<Transform>();
        space->AddComponent<Sizeable>()->Size = {size.x, 10};
        space->AddComponent<Layoutable>();
        
        for (auto field : fields.fields) {
            if (!field->HasEditor()) continue;
            
            GameObject* editor = world->CreateObject();
            editor->Parent = componentChild;
            editor->AddComponent<Transform>();
            editor->AddComponent<Sizeable>()->Size = {size.x, size.y*0.5f};
            editor->AddComponent<Layoutable>();
            FieldEditor* fieldEditor = editor->AddComponent<FieldEditor>();
            fieldEditor->Object = component;
            fieldEditor->Field = field->name;
        }
        
        GameObject* componentName = gui->CreateLabel(componentChild, 0, {size.x, size.y*0.5f}, 0, componentNames[i], 14);
        componentName->GetComponent<Label>()->HAlignment = Font::Center;
        componentName->GetComponent<Label>()->VAlignment = Font::Middle;
        //componentName->GetComponent<Colorable>()->Color = Colour::Black();
        componentName->AddComponent<Layoutable>();
    }
     */
}