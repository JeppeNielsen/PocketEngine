//
//  EditorSizeableSelection.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 02/06/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "EditorSizeableSelection.hpp"



void EditorSizeableSelection::CreateSubSystems(GameStorage& storage) {
    storage.AddSystemType<SizeableLineRenderer>();
}

void EditorSizeableSelection::Initialize() {
    root->CreateSystem<SizeableLineRenderer>();
}

void EditorSizeableSelection::CreateObject(GameObject* object, GameObject* selectionObject) {
    selectionObject->AddComponent<Sizeable>(object);
    selectionObject->AddComponent<Transform>(object);
    selectionObject->AddComponent<LineRenderer>();
}

void EditorSizeableSelection::SizeableLineRenderer::ObjectAdded(GameObject* object) {
    Sizeable* sizeable = object->GetComponent<Sizeable>();
    sizeable->Size.Changed.Bind(this, &EditorSizeableSelection::SizeableLineRenderer::SizeChanged, object);
    dirtyObjects.insert(object);
}

void EditorSizeableSelection::SizeableLineRenderer::ObjectRemoved(GameObject* object) {
    Sizeable* sizeable = object->GetComponent<Sizeable>();
    sizeable->Size.Changed.Unbind(this, &EditorSizeableSelection::SizeableLineRenderer::SizeChanged, object);
    dirtyObjects.insert(object);
    auto it = dirtyObjects.find(object);
    if (it!=dirtyObjects.end()) {
        dirtyObjects.erase(it);
    }
}

void EditorSizeableSelection::SizeableLineRenderer::SizeChanged(GameObject* object) {
    dirtyObjects.insert(object);
}

void EditorSizeableSelection::SizeableLineRenderer::Update(float dt) {
    for(auto o : dirtyObjects) {
        Sizeable* sizeable = o->GetComponent<Sizeable>();
        LineRenderer* lineRenderer = o->GetComponent<LineRenderer>();
        auto& v = lineRenderer->vertices;
        if (v.size()!=8) {
            v.resize(8);
        }
        
        v[0] = {0,0,0};
        v[1] = {sizeable->Size().x, 0, 0 };
        
        v[2] = {sizeable->Size().x, 0, 0 };
        v[3] = {sizeable->Size().x, sizeable->Size().y,0};
        
        
        v[4] = {sizeable->Size().x, sizeable->Size().y,0};
        v[5] = {0, sizeable->Size().y, 0 };
        
        v[6] = {0, sizeable->Size().y, 0 };
        v[7] = {0, 0, 0 };

    }
    dirtyObjects.clear();
}
