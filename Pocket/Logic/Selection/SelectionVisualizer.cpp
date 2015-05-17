//
//  SelectionVisualizer.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/26/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "SelectionVisualizer.hpp"
#include "GameWorld.hpp"
#include "Material.hpp"

using namespace Pocket;

void SelectionVisualizer::Initialize() {
    AddComponent<Transform>();
    AddComponent<Mesh>();
    AddComponent<Selectable>();
}

void SelectionVisualizer::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed += event_handler(this, &SelectionVisualizer::SelectionChanged, object);
}

void SelectionVisualizer::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed -= event_handler(this, &SelectionVisualizer::SelectionChanged, object);
    Selections::iterator it = selections.find(object);
    
    //GameObject* test =0;
    //test->GetComponent<Transform>();
    
    if (it!=selections.end()) {
        it->second->Remove();
        
        
        selections.erase(it);
    }
}

GameObject* SelectionVisualizer::CreateSelection(GameObject* object) {
    GameObject* go = World()->CreateObject();
    
    go->AddComponent<Transform>(object);
    go->AddComponent<Mesh>()->AddCube(object->GetComponent<Mesh>()->LocalBoundingBox.GetValue()->center, object->GetComponent<Mesh>()->LocalBoundingBox.GetValue()->extends * 0.5f + Vector3(0.01f,0.01f,0.01f), Colour(0.0f, 0.0f, 1.0f, 0.5f));
    go->AddComponent<Material>()->BlendMode = BlendMode::Alpha;
    
    return go;
}

void SelectionVisualizer::SelectionChanged(Pocket::Selectable *selectable, GameObject* object) {
    if (selectable->Selected) {
        selections[object] = CreateSelection(object);
    } else {
        Selections::iterator it = selections.find(object);
        if (it!=selections.end()) {
            it->second->Remove();
            selections.erase(it);
        }
    }
}