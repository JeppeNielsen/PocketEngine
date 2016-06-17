//
//  GameSystem.hpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 08/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "GameIDHelper.hpp"
#include "GameObject.hpp"

namespace Pocket {
    
    class GameWorld;
    class IGameSystem {
    protected:
        GameWorld* const world;
        IGameSystem();
        virtual ~IGameSystem();
        void TryAddComponentContainer(ComponentID id, std::function<IContainer*()> constructor);
        friend class GameWorld;
        virtual void Initialize();
        virtual void ObjectAdded(GameObject* object);
        virtual void ObjectRemoved(GameObject* object);
        virtual void Update(float dt);
        virtual void Render();
    private:
        ObjectCollection objects;
        ComponentMask componentMask;
        friend class GameObject;
    public:
        const ObjectCollection& Objects() const;
    };
    
    template<typename ...T>
    class GameSystem : public IGameSystem {
    private:
        template<typename Last>
        void ExtractComponents(std::vector<int>& components) {
            ComponentID id = GameIDHelper::GetComponentID<Last>();
            TryAddComponentContainer(id, [](){ return new Container<Last>(); });
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
}