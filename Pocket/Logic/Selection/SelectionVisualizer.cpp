//
//  SelectionVisualizer.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 12/26/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "SelectionVisualizer.hpp"
#include "GameSystem.hpp"
#include "Renderable.hpp"

using namespace Pocket;


void SelectionVisualizer::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Bind(this, &SelectionVisualizer::SelectionChanged, object);
}

void SelectionVisualizer::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Unbind(this, &SelectionVisualizer::SelectionChanged, object);
    Selections::iterator it = selections.find(object);
    
    //GameObject* test =0;
    //test->GetComponent<Transform>();
    
    if (it!=selections.end()) {
        it->second->Remove();
        
        
        selections.erase(it);
    }
}

GameObject* SelectionVisualizer::CreateSelection(GameObject* object) {
    GameObject* go = object->CreateObject();
    
    go->AddComponent<Transform>(object);
    go->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(object->GetComponent<Mesh>()->LocalBoundingBox().center, object->GetComponent<Mesh>()->LocalBoundingBox().extends * 0.5f + Vector3(0.01f,0.01f,0.01f), Colour(0.0f, 0.0f, 1.0f, 0.5f));
    go->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
    
    return go;
}

void SelectionVisualizer::SelectionChanged(GameObject* object) {
    Selectable *selectable = object->GetComponent<Selectable>();
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
