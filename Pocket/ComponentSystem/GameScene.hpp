//
//  GameScene.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 18/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "GameObject.hpp"
#include "Property.hpp"

namespace Pocket {
    class GameWorld;
    class IGameSystem;
    class ScriptWorld;
    class GameScene {
    private:
        friend class GameWorld;
        friend class GameObject;
        friend class std::allocator<GameScene>;
        friend class ScriptWorld;
        
        using Actions = std::deque<std::function<void()>>;
        Actions delayedActions;
        
        GameWorld* world;
        GameObject* root;
        int index;
        std::vector<IGameSystem*> systemsIndexed;
        std::vector<IGameSystem*> activeSystems;
        int idCounter;
        std::string guid;
        
        GameScene();
        ~GameScene();
        GameScene(const GameScene& other);
        void DoActions(Actions &actions);
        void Update(float dt);
        void Render();
        void DestroySystems();
        GameObject* FindObject(int objectId);
        
        IGameSystem* CreateSystem(int systemId);
        
        friend class Container<GameScene>;
    };
}