//
//  EditorObjectCreatorSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 14/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "EditorObject.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Selectable.hpp"

using namespace Pocket;

class EditorObjectCreatorSystem : public GameSystem<EditorObject> {
public:
    void Initialize() override;
    void ObjectAdded(GameObject* object) override;
    void ObjectRemoved(GameObject* object) override;
    //int Order() override;
    
    GameObject* editorRoot;
    
private:
    template<typename T>
    struct ComponentSystem : public GameSystem<EditorObject, T> {
        void ObjectAdded(GameObject* object) {
            GameObject* editorObject = (GameObject*)creatorSystem->GetMetaData(object);
            editorObject->AddComponent<T>(object);
        }

        void ObjectRemoved(Pocket::GameObject *object) {
            GameObject* editorObject = (GameObject*)creatorSystem->GetMetaData(object);
            editorObject->RemoveComponent<T>();
        }

        EditorObjectCreatorSystem* creatorSystem;
    };
};