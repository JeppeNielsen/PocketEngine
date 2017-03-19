//
//  EditorWorld.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/03/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include <set>

using namespace Pocket;

class EditorScene {
public:
    EditorScene();
    void Initialize(GameObject* object);
    void Destroy();
    
    GameObject* GameRoot();
    GameObject* EditorRoot();

private:
    GameObject* editorRoot;
    GameObject* sceneRoot;
    
    std::map<GameObject*, GameObject*> rootToEditorMap;
    std::vector<ComponentId> editorObjectsComponents;
    
    struct Proxy {
        std::function<void(GameObject*, GameObject*)> add;
        std::function<void(GameObject*)> remove;
    };
    
    std::map<ComponentId, Proxy> proxyComponents;
    
    bool IsClonerInAncestry(GameObject* object);
    void CreateEditorSystems(Pocket::GameObject &editorWorld);
    void BindToRoot(GameObject* root);
    void AddEditorObject(GameObject* object);
    GameObject* AddObjectToEditor(GameObject* rootObject);
};
