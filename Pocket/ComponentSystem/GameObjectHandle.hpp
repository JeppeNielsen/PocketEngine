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

namespace Pocket {
    class GameObjectHandle {
    public:
        GameObjectHandle();
        
        GameObject* operator -> ();
        void operator = (GameObject& v);
        void operator = (GameObject* v);
        
        explicit operator bool() {
            return operator->();
        }
        
        GameObject* operator() ();
        operator GameObject* ();
    
        void SetRoot(GameObject* root);
        
        inline friend std::ostream& operator<<(std::ostream& stream, const GameObjectHandle& handle) {
            stream<<handle.sceneGuid<<":"<<handle.rootId;
            return stream;
        }
        
        GameObjectHandle static Deserialize(const std::string& data);
    
    private:
    
        void Set(GameObject* ptr);
        GameObject* Get();
        
        GameWorld* world;
        int index;
        int version;
        int rootId;
        std::string sceneGuid;
    
    };
}
