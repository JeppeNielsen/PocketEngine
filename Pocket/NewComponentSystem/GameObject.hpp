//
//  GameObject.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 17/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "Property.hpp"
#include "DirtyProperty.hpp"
#include "IGameObject.hpp"
#include "InputManager.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    class GameWorld;
    class GameScene;
    class GameObject;
    class IGameSystem;
    
    using ObjectCollection = std::vector<GameObject*>;
    
    using SerializePredicate = std::function<bool(const GameObject*, int)>;
    
    class GameObject : public IGameObject {
    private:
        friend class GameWorld;
        friend class GameScene;
        friend class Handle<GameObject>;
        friend class Container<GameObject>;
        friend class std::allocator<GameObject>;
        friend class ScriptWorld;
        
        using ComponentIndicies = std::vector<int>;
        
        GameScene* scene;
        ComponentIndicies componentIndicies;
        
        Bitset activeComponents;
        Bitset enabledComponents;
    public:
        Property<GameObject*> Parent;
        
    private:
        ObjectCollection children;
        
        bool removed;
        int index;
        int rootId;
        
    public:
        Property<bool> Enabled;
        DirtyProperty<bool> WorldEnabled;
        Property<int> Order;
        
    private:
    
        GameObject();
        ~GameObject();
        GameObject(const GameObject& other);
    
        void Reset();
        void TrySetComponentEnabled(ComponentId id, bool enable);
        void SetWorldEnableDirty();
        void SetEnabled(bool enabled);
        IGameSystem* GetSystem(SystemId id);
        
        void WriteJson(minijson::object_writer& writer, SerializePredicate predicate) const;
        void SerializeComponent(int componentID, minijson::array_writer& writer, bool isReference, const GameObject* referenceObject) const;
        void AddComponent(minijson::istream_context& context, std::string componentName);
        
        static bool GetAddReferenceComponent(Pocket::GameObject **object, int &componentID, Pocket::GameObject** referenceObject);
        static void EndGetAddReferenceComponent();
    public:
        
        bool HasComponent(ComponentId id) const override;
        void* GetComponent(ComponentId id) const override;
        void AddComponent(ComponentId id) override;
        void AddComponent(ComponentId id, GameObject* referenceObject) override;
        void RemoveComponent(ComponentId id) override;
        void CloneComponent(ComponentId id, GameObject* object) override;
        
        template<typename T>
        bool HasComponent() const {
            return HasComponent(GameIdHelper::GetComponentID<T>());
        }
        
        template<typename T>
        T* GetComponent() const {
            return static_cast<T*>(GetComponent(GameIdHelper::GetComponentID<T>()));
        }
        
        template<typename T>
        T* AddComponent() {
            ComponentId componentId = GameIdHelper::GetComponentID<T>();
            AddComponent(componentId);
            return static_cast<T*>(GetComponent(componentId));
        }
        
        template<typename T>
        T* AddComponent(GameObject* source) {
            ComponentId componentId = GameIdHelper::GetComponentID<T>();
            AddComponent(componentId, source);
            return static_cast<T*>(GetComponent(componentId));
        }
        
        template<typename T>
        void RemoveComponent() {
            RemoveComponent(GameIdHelper::GetComponentID<T>());
        }
        
        template<typename T>
        T* CloneComponent(GameObject* source) {
            ComponentId componentId = GameIdHelper::GetComponentID<T>();
            CloneComponent(componentId, source);
            return static_cast<T*>(GetComponent(componentId));
        }
        
        template<typename T>
        T* GetSystem() {
            return static_cast<T*>(GetSystem(GameIdHelper::GetSystemID<T>()));
        }
        
        std::vector<TypeInfo> GetComponentTypes(const std::function<bool(int componentID)>& predicate);
        std::vector<int> GetComponentIndicies();
        
        InputManager& Input();
        
        void Remove();
        bool IsRemoved() const;
        
        GameObject* CreateChild();
        GameObject* CreateObject();
        GameObject* Root();
        GameObject* CreateChildFromJson(std::istream& jsonStream, std::function<void(GameObject*)> onCreated);
        
        void ToJson(std::ostream& stream, SerializePredicate predicate = 0) const;
        
        bool IsRoot() const;
        
        const ObjectCollection& Children();
        
        Handle<GameObject> GetHandle();
        
        int RootId() const;
    };
    
    
}