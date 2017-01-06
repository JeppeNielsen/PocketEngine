//
//  EditorTransformSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 10/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

//
//  EditorSelectionSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorTransformSystem.hpp"
#include "Selectable.hpp"
#include "Touchable.hpp"
#include "Draggable.hpp"
#include "Renderable.hpp"

void EditorTransformSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Bind(this, &EditorTransformSystem::SelectionChanged, object);
}

void EditorTransformSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Unbind(this, &EditorTransformSystem::SelectionChanged, object);
    TryRemoveTransformObject(object);
}

void EditorTransformSystem::SelectionChanged(Pocket::GameObject *object) {
    if (object->GetComponent<Selectable>()->Selected) {
        GameObject* transformerObject = root->CreateObject();
        transformerObject->AddComponent<Transform>(object);
        
        {
            GameObject* xaxis = transformerObject->CreateChild();
            xaxis->AddComponent<Transform>(object);
            xaxis->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({1,0,0}, {1,0.05f,0.05f});
            xaxis->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour(1.0f,0,0.0f, 0.5f));
            xaxis->AddComponent<Renderable>();
            xaxis->AddComponent<Touchable>();
            xaxis->AddComponent<Draggable>()->Movement = Draggable::MovementMode::XAxis;
            xaxis->AddComponent<Selectable>(object);
        }
        
        {
            GameObject* yaxis = transformerObject->CreateChild();
            yaxis->AddComponent<Transform>(object);
            yaxis->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({0,1,0}, {0.05f,1.0f,0.05f});
            yaxis->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour(0.0f,1.0f,0.0f, 0.5f));
            yaxis->AddComponent<Renderable>();
            yaxis->AddComponent<Touchable>();
            yaxis->AddComponent<Draggable>()->Movement = Draggable::MovementMode::YAxis;
            yaxis->AddComponent<Selectable>(object);
        }
        
        {
            GameObject* zaxis = transformerObject->CreateChild();
            zaxis->AddComponent<Transform>(object);
            zaxis->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({0,0,1}, {0.05f,0.05f,1.0f});
            zaxis->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour(0.0f,0.0f,1.0f, 0.5f));
            zaxis->AddComponent<Renderable>();
            zaxis->AddComponent<Touchable>();
            zaxis->AddComponent<Draggable>()->Movement = Draggable::MovementMode::ZAxis;
            zaxis->AddComponent<Selectable>(object);
        }
        transformObjects[object] = transformerObject;
    } else {
        TryRemoveTransformObject(object);
    }
}
    
void EditorTransformSystem::TryRemoveTransformObject(Pocket::GameObject *object) {
    auto it = transformObjects.find(object);
    if (it!=transformObjects.end()) {
        it->second->Remove();
        transformObjects.erase(it);
    }
}
