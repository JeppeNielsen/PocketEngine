//
//  GameScene.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 18/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <functional>
#include "GameObject.hpp"
#include "Property.hpp"

namespace Pocket {
    class GameWorld;
    class IGameSystem;
    class ScriptWorld;
    class GameObjectHandle;
    class GameScene {
    private:
        friend class GameWorld;
        friend class GameObject;
        friend class std::allocator<GameScene>;
        friend class ScriptWorld;
        friend class GameObjectHandle;
        
        GameWorld* world;
        GameObject* root;
        int index;
        std::vector<IGameSystem*> systemsIndexed;
        int idCounter;
        std::string guid;
        Property<float> timeScale;
        Property<bool> updateEnabled;
        Property<bool> renderEnabled;
        
        GameScene();
        ~GameScene();
        GameScene(const GameScene& other);
        void Update(float dt);
        void Render();
        void DestroySystems();
        GameObject* FindObject(int objectId);
        
        IGameSystem* CreateSystem(int systemId);
        void RemoveSystem(int systemId);
        
        void IterateObjects(const std::function<void(GameObject*)>& callback);
        
        friend class Container<GameScene>;
    };
}