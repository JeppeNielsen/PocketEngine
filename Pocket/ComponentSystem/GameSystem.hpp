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
#include "GameStorage.hpp"
#include "GameObjectHandle.hpp"

namespace Pocket {
    
    class GameSystemBase : public IGameSystem {
    protected:
        GameObject* const root;
        friend class GameStorage;
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
        friend class GameStorage;
    public:
        Property<int> Order;
        const ObjectCollection& Objects() const;
    };
    
    template<typename ...T>
    class GameSystem : public GameSystemBase {
    private:
        template<typename Last>
        static void ExtractComponents(GameStorage& storage, std::vector<ComponentId>& components) {
            storage.AddComponentType<Last>();
            ComponentId id = GameIdHelper::GetComponentID<Last>();
            components.push_back(id);
        }
        
        template<typename First, typename Second, typename ...Rest>
        static void ExtractComponents(GameStorage& storage, std::vector<int>& components) {
            ExtractComponents<First>(storage, components);
            ExtractComponents<Second, Rest...>(storage, components);
        }
    
        static void ExtractAllComponents(GameStorage& storage, std::vector<ComponentId>& components) {
            ExtractComponents<T...>(storage, components);
        }
        
        friend class GameStorage;
    };
    
    class GameConcept : public GameSystemBase {
    private:
        static void ExtractAllComponents(GameStorage& storage, std::vector<int>& components) { }
        friend class GameStorage;
    };
}
