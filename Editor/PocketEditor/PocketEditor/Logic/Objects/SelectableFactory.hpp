//
//  SelectableFactory.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 11/04/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include "Selectable.hpp"
#include <map>

using namespace Pocket;

class SelectableFactory : public GameConcept {
public:
    void Initialize(GameWorld* world);
    GameObject* AddSelectable(GameObject* objectInGame);
    void RemoveSelectable(GameObject* objectInGame);
    Selectable* GetSelectable(GameObject* objectInGame);
    GameObject* GetGameObject(GameObject* selectedObject);
    
private:
    GameWorld* world;
    
    struct SelectableObject {
        int references;
        GameObject* object;
    };
    
    using Selectables = std::map<GameObject*, SelectableObject>;
    Selectables selectables;
};

