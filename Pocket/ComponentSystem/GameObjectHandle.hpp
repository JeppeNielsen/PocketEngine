//
//  GameObjectHandle.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 12/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameObject.hpp"
#include "GameWorld.hpp"
#include "Event.hpp"

namespace Pocket {
    class GameObjectHandle {
    public:
        GameObjectHandle();
        
        GameObject* operator -> ();
        void operator = (const GameObject& v);
        void operator = (GameObject* v);
        void operator = (const GameObjectHandle& handle);
        
        explicit operator bool() {
            return operator->();
        }
        
        GameObject* operator() ();
        operator GameObject* ();
    
        void SetRoot(const GameObject* root);
        
        inline friend std::ostream& operator<<(std::ostream& stream, const GameObjectHandle& handle) {
            stream<<handle.sceneGuid<<":"<<handle.rootId;
            return stream;
        }
        
        GameObjectHandle static Deserialize(const std::string& data);
        
        Event<> Changed;
        
        const std::string& SceneGuid();
        
        bool HasRoot();
    
    private:
    
        void Set(const GameObject* ptr);
        GameObject* Get();
        void SetWorld(GameWorld* world);
        
        GameWorld* world;
        int index;
        int version;
        int rootId;
        std::string sceneGuid;
        
        friend class GameWorld;
    };
}
