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
#include "GameWorld.hpp"

namespace Pocket {
    
    class GameSystemBase : public IGameSystem {
    protected:
        GameObject* const root;
        friend class GameWorld;
        GameSystemBase();
        virtual ~GameSystemBase();
        virtual void Initialize() override;
        virtual void Destroy() override;
        virtual void ObjectAdded(GameObject* object) override;
        virtual void ObjectRemoved(GameObject* object) override;
        virtual void Update(float dt) override;
        virtual void Render() override;
        
        int AddObject(GameObject* object) override;
        void RemoveObject(GameObject* object) override;
        
        void SetMetaData(GameObject* object, void* data);
        void* GetMetaData(GameObject* object);
        
        int GetOrder() override { return Order(); }
        void SetOrder(int order) override { Order = order; }
        int GetIndex() override { return index; }
        void SetIndex(int index) override { this->index = index; }
    
    public:
        int ObjectCount() override;
        
    private:
        using MetaData = std::map<GameObject*, void*>;
        MetaData metaData;
        
        int index;
    
        ObjectCollection objects;
        friend class GameWorld;
    public:
        Property<int> Order;
        const ObjectCollection& Objects() const;
    };
    
    template<typename ...T>
    class GameSystem : public GameSystemBase {
    private:
        template<typename Last>
        static void ExtractComponents(GameWorld& world, std::vector<ComponentId>& components) {
            world.AddComponentType<Last>();
            ComponentId id = GameIdHelper::GetComponentID<Last>();
            components.push_back(id);
        }
        
        template<typename First, typename Second, typename ...Rest>
        static void ExtractComponents(GameWorld& world, std::vector<int>& components) {
            ExtractComponents<First>(world, components);
            ExtractComponents<Second, Rest...>(world, components);
        }
    
        static void ExtractAllComponents(GameWorld& world, std::vector<ComponentId>& components) {
            ExtractComponents<T...>(world, components);
        }
        
        friend class GameWorld;
    };
    
    class GameConcept : public GameSystemBase {
    private:
        static void ExtractAllComponents(GameWorld& world, std::vector<int>& components) {
        }
        friend class GameWorld;
    };
}
