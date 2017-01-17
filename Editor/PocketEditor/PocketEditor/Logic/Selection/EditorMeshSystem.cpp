//
//  EditorMeshSystem.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "EditorMeshSystem.hpp"
#include "Renderable.hpp"

void EditorMeshSystem::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Bind(this, &EditorMeshSystem::SelectionChanged, object);
}

void EditorMeshSystem::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Selectable>()->Selected.Changed.Unbind(this, &EditorMeshSystem::SelectionChanged, object);
    TryRemoveTransformObject(object);
}

void EditorMeshSystem::SelectionChanged(Pocket::GameObject *object) {
    if (object->GetComponent<Selectable>()->Selected) {
        GameObject* selectionObject = root->CreateObject();
        selectionObject->AddComponent<Transform>(object);
    
        object->GetComponent<Mesh>()->LocalBoundingBox.HasBecomeDirty.Bind(this, &EditorMeshSystem::BoundingBoxDirty, object);
        
        selectionObject->AddComponent<Mesh>()->GetMesh<Vertex>();
        selectionObject->AddComponent<Renderable>()->BlendMode = BlendModeType::Alpha;
        
        dirtyObjects.insert(object);
        transformObjects[object] = selectionObject;
    } else {
       TryRemoveTransformObject(object);
    }
}

void EditorMeshSystem::TryRemoveTransformObject(Pocket::GameObject *object) {
    auto it = transformObjects.find(object);
    if (it!=transformObjects.end()) {
        it->second->Remove();
        transformObjects.erase(it);
        
        auto it2 = dirtyObjects.find(object);
        if (it2 != dirtyObjects.end()) {
            dirtyObjects.erase(it2);
        }
        object->GetComponent<Mesh>()->LocalBoundingBox.HasBecomeDirty.Unbind(this, &EditorMeshSystem::BoundingBoxDirty, object);
    }
}

void EditorMeshSystem::BoundingBoxDirty(Pocket::GameObject *object) {
    dirtyObjects.insert(object);
}

void EditorMeshSystem::Update(float dt) {
    for(auto o : dirtyObjects) {
        CreateMesh(o);
    }
    dirtyObjects.clear();
}

void EditorMeshSystem::CreateMesh(Pocket::GameObject *object) {
    GameObject* selectionObject = transformObjects[object];
    
    auto& boundingBox = object->GetComponent<Mesh>()->LocalBoundingBox();
    
    selectionObject->AddComponent<Mesh>()->GetMesh<Vertex>().Clear();
    selectionObject->AddComponent<Mesh>()->GetMesh<Vertex>().AddCube(boundingBox.center, boundingBox.extends*0.5f+0.05f);
    selectionObject->GetComponent<Mesh>()->GetMesh<Vertex>().SetColor(Colour(0,0,1.0f, 0.5f));
}
