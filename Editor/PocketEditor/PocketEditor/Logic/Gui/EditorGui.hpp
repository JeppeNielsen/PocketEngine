//
//  EditorGui.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 29/08/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//


#pragma once
#include "GameSystem.hpp"
#include "VirtualTreeListSpawner.hpp"

namespace Pocket {
    class Gui;
};

class EditorGui : public Pocket::GameConcept {
protected:
    void Initialize();
    Pocket::Gui* gui;
public:
    Pocket::GameObject* CreateTreeList(Pocket::GameObject* parent, Pocket::GameObject* root,
        const std::function<std::string(Pocket::GameObject*)>& expandedHashFunction,
        const std::function<bool(Pocket::GameObject* object)>& predicateFunction,
        const std::function<bool(Pocket::GameObject*)>& hasChildren,
        const std::function<void(Pocket::VirtualTreeListSpawner::SpawnedNode& node, Pocket::GameObject* button, std::string& text)>& OnCreate,
        const std::function<void(const Pocket::VirtualTreeListSpawner::SpawnedNode& node, Pocket::GameObject* button)>& OnRemove
        );
};
