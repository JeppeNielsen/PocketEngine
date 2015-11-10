//
//  EditorStateSystem.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 09/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "EditorState.h"

using namespace Pocket;

template<class Object, class Component>
class EditorStateSystem : public GameSystem {
    public:
        EditorState::State OnState;
    
    void Initialize() {
        AddComponent<EditorState>();
        AddComponent<Object>();
    }

    void ObjectAdded(GameObject* object) {
        object->GetComponent<EditorState>()->CurrentState.Changed += event_handler(this, &EditorStateSystem<Object, Component>::EditorStateChanged, object);
        EditorStateChanged(object->GetComponent<EditorState>(), object);
    }
    
    void ObjectRemoved(GameObject* object) {
        object->GetComponent<EditorState>()->CurrentState.Changed -= event_handler(this, &EditorStateSystem<Object, Component>::EditorStateChanged, object);
    }
    
    void EditorStateChanged(EditorState* editorState, GameObject* object) {
        if (editorState->CurrentState == OnState) {
            if (!object->GetComponent<Component>()) {
                object->AddComponent<Component>();
            }
        } else {
            if (object->GetComponent<Component>()) {
                object->RemoveComponent<Component>();
            }
        }
    }
};


template<class Object, class Component>
class EditorStateEnablerSystem : public GameSystem {
    public:
        EditorState::State OnState;
    
    void Initialize() {
        AddComponent<EditorState>();
        AddComponent<Object>();
    }

    void ObjectAdded(GameObject* object) {
        object->GetComponent<EditorState>()->CurrentState.Changed += event_handler(this, &EditorStateEnablerSystem<Object, Component>::EditorStateChanged, object);
        EditorStateChanged(object->GetComponent<EditorState>(), object);
    }
    
    void ObjectRemoved(GameObject* object) {
        object->GetComponent<EditorState>()->CurrentState.Changed -= event_handler(this, &EditorStateEnablerSystem<Object, Component>::EditorStateChanged, object);
    }
    
    void EditorStateChanged(EditorState* editorState, GameObject* object) {
        object->EnableComponent<Component>(editorState->CurrentState == OnState);
    }
};
