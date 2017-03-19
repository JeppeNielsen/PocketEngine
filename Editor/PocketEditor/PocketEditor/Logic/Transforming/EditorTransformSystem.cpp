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
#include "LineRenderer.hpp"
#include "DistanceScaler.hpp"
#include "Orderable.hpp"
#include "Colorable.hpp"

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
        
        {
            GameObject* xaxis = transformerObject->CreateChild();
            xaxis->AddComponent<Transform>(object);
            
            xaxis->AddComponent<Draggable>()->Movement = Draggable::MovementMode::XAxis;
            xaxis->AddComponent<Selectable>(object);
            
            
            GameObject* visual = xaxis->CreateChild();
            visual->AddComponent<Transform>();
            visual->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({0.5f,0,0}, {0.5f,0.2f,0.2f});
            //visual->AddComponent<Renderable>();
            visual->AddComponent<Touchable>();
            visual->AddComponent<DistanceScaler>()->Scale = 0.1f;
            //visual->AddComponent<Orderable>()->Order = -100;
            
            visual->AddComponent<Colorable>()->Color = Colour(1.0f,0.0f,0.0f, 1.0f);
            visual->AddComponent<LineRenderer>()->vertices = { Vector3(0), Vector3(1,0,0) };
            visual->GetComponent<LineRenderer>()->Width = 10;
            
            xaxis->AddComponent<Touchable>(visual);
        }
        
        {
            GameObject* xaxis = transformerObject->CreateChild();
            xaxis->AddComponent<Transform>(object);
            
            xaxis->AddComponent<Draggable>()->Movement = Draggable::MovementMode::YAxis;
            xaxis->AddComponent<Selectable>(object);

            GameObject* visual = xaxis->CreateChild();
            visual->AddComponent<Transform>();
            visual->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({0,0.5f,0}, {0.2f,0.5f,0.2f});
            //visual->AddComponent<Renderable>();
            visual->AddComponent<Touchable>();
            visual->AddComponent<DistanceScaler>()->Scale = 0.1f;
            //visual->AddComponent<Orderable>()->Order = -100;
            
            visual->AddComponent<Colorable>()->Color = Colour(0.0f,1,0.0f, 1.0f);
            visual->AddComponent<LineRenderer>()->vertices = { Vector3(0), Vector3(0,1,0) };
            visual->GetComponent<LineRenderer>()->Width = 10;
            
            xaxis->AddComponent<Touchable>(visual);

        }
        
        {
            GameObject* xaxis = transformerObject->CreateChild();
            xaxis->AddComponent<Transform>(object);
            
            xaxis->AddComponent<Draggable>()->Movement = Draggable::MovementMode::ZAxis;
            xaxis->AddComponent<Selectable>(object);
            
            GameObject* visual = xaxis->CreateChild();
            visual->AddComponent<Transform>();
            visual->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube({0,0,0.5f}, {0.2f,0.2f,0.5f});
            //visual->AddComponent<Renderable>();
            visual->AddComponent<Touchable>();
            visual->AddComponent<DistanceScaler>()->Scale = 0.1f;
            //visual->AddComponent<Orderable>()->Order = -100;
            
            visual->AddComponent<Colorable>()->Color = Colour(0.0f,0.0f,1.0f,1.0f);
            visual->AddComponent<LineRenderer>()->vertices = { Vector3(0), Vector3(0,0,1) };
            visual->GetComponent<LineRenderer>()->Width = 10;
            
            xaxis->AddComponent<Touchable>(visual);
            

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
