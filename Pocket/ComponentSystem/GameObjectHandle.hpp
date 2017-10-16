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
#include "TypeInterpolator.hpp"

namespace Pocket {

    class IGameObjectHandleRetriever {
    public:
        virtual GameObject* Get(GameWorld* world, int index, int version, int rootId, std::string sceneGuid) = 0;
    };
    
    class GameObjectHandleRetriever : public IGameObjectHandleRetriever {
    public:
        GameObject* Get(GameWorld* world, int index, int version, int rootId, std::string sceneGuid) override;
    };
    
    class GameObjectHandle {
    public:
        GameObjectHandle();
        GameObjectHandle(const GameObjectHandle& other);
        GameObjectHandle(GameObject* object);
        ~GameObjectHandle();
        
        GameObject* operator -> ();
        void operator = (const GameObject& v);
        void operator = (GameObject* v);
        void operator = (const GameObjectHandle& handle);
        
        explicit operator bool() {
            return operator->();
        }
        
        inline bool operator ==(const GameObjectHandle &other) const{
            return !(index!=other.index || version!=other.version || rootId!=other.version || sceneGuid!=other.sceneGuid);
        }
        
        inline  bool operator !=(const GameObjectHandle &other) const{
            return (index!=other.index || version!=other.version || rootId!=other.version || sceneGuid!=other.sceneGuid);
        }
        
        GameObject* operator() ();
        operator GameObject* ();
            
        inline friend std::ostream& operator<<(std::ostream& stream, const GameObjectHandle& handle) {
            stream<<handle.sceneGuid<<":"<<handle.rootId;
            return stream;
        }
        
        GameObjectHandle static Deserialize(const std::string& data);
        
        Event<> Changed;
        
        const std::string& SceneGuid();
        
        bool HasRoot();
        
        void SetWorld(GameWorld* world);
    
    private:
    
        void Set(const GameObject* ptr);
        GameObject* Get();
        
        
        GameWorld* world;
        int index;
        int version;
        int rootId;
        std::string sceneGuid;
        
        friend class GameWorld;
        friend class GameObjectHandleRetriever;
        friend class GameObject;
    };
    
    template<>
    struct TypeInterpolator<GameObjectHandle> {
        static void Interpolate(GameObjectHandle* value, float t, const GameObjectHandle& a, const GameObjectHandle& b) {
            value->operator=(t<0.5f ? a : b);
        }
    };
}
