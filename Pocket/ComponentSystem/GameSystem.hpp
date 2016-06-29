//
//  GameSystem.hpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 08/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <map>
#include "GameIDHelper.hpp"
#include "GameObject.hpp"

namespace Pocket {
    
    class GameWorld;
    
    struct IGameSystem {
        virtual ~IGameSystem() = default;
        virtual void Initialize() = 0;
        virtual void ObjectAdded(GameObject* object) = 0;
        virtual void ObjectRemoved(GameObject* object) = 0;
        virtual void Update(float dt) = 0;
        virtual void Render() = 0;
        virtual int AddObject(GameObject* object) = 0;
        virtual void RemoveObject(GameObject* object) = 0;
    };
    
    class GameSystemBase : public IGameSystem {
    protected:
        GameWorld* const world;
        friend class GameWorld;
        GameSystemBase();
        virtual ~GameSystemBase();
        void TryAddComponentContainer(ComponentID id, std::function<IContainer*(std::string&)>&& constructor);
        
        virtual void Initialize();
        virtual void ObjectAdded(GameObject* object);
        virtual void ObjectRemoved(GameObject* object);
        virtual void Update(float dt);
        virtual void Render();
        
        int AddObject(GameObject* object);
        void RemoveObject(GameObject* object);
        
        void SetMetaData(GameObject* object, void* data);
        void* GetMetaData(GameObject* object);
        
    private:
        using MetaData = std::map<GameObject*, void*>;
        MetaData metaData;
    
        ObjectCollection objects;
        Bitset componentMask;
        friend class GameObject;
    public:
        const ObjectCollection& Objects() const;
    };
    
    template<typename ...T>
    class GameSystem : public GameSystemBase {
    private:
        template<typename Last>
        void ExtractComponents(std::vector<int>& components) {
            ComponentID id = GameIDHelper::GetComponentID<Last>();
            TryAddComponentContainer(id, [](std::string& componentName){
                componentName = GameIDHelper::GetClassName<Last>();
                return new Container<Last>();
            });
            components.push_back(id);
        }
        
        template<typename First, typename Second, typename ...Rest>
        void ExtractComponents(std::vector<int>& components) {
            ExtractComponents<First>(components);
            ExtractComponents<Second, Rest...>(components);
        }
    
        void ExtractAllComponents(std::vector<int>& components) {
            ExtractComponents<T...>(components);
        }
        
        friend class GameWorld;
    };
    
    class GameConcept : public GameSystemBase {
    private:
        void ExtractAllComponents(std::vector<int>& components) {
        }
        friend class GameWorld;
    };
}