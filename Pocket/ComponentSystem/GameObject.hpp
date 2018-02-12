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
#include "Hierarchy.hpp"

namespace Pocket {
    class GameWorld;
    class GameScene;
    class GameObject;
    class IGameSystem;
    class GameObjectHandle;
    class GameStorage;
    class GameObjectJsonSerializer;
    
    using ObjectCollection = std::vector<GameObject*>;
    
    using SerializePredicate = std::function<bool(const GameObject*, int)>;
    
    class GameObject : public IGameObject {
    private:
        friend class GameWorld;
        friend class GameScene;
        friend class Container<GameObject>;
        friend class std::allocator<GameObject>;
        friend class ScriptWorld;
        friend class GameObjectHandle;
        friend class GameStorage;
        friend class GameObjectJsonSerializer;
        friend class Hierarchy;
        
        using ComponentIndicies = std::vector<int>;
        
        GameScene* scene;
        ComponentIndicies componentIndicies;
        
        Bitset activeComponents;
        Bitset enabledComponents;
        
        bool removed;
        int index;
        int id;
        
    public:
    
        struct ReferenceComponent {
            GameObject* object;
            int componentId;
            std::string name;
        };
        
    private:
        
        struct CloneReferenceComponent {
            GameObject* object;
            int componentId;
            int referenceObjectId;
        };
    
        GameObject();
        ~GameObject();
        GameObject(const GameObject& other);
    
        void Reset();
        void TrySetComponentEnabled(ComponentId id, bool enable);
        void SetWorldEnableDirty();
        void SetEnabled(bool enabled);
        void TryAddToSystem(int systemId);
        void TryRemoveFromSystem(int systemId);
        
        void RemoveComponents(const SerializePredicate& predicate);
        
        void ApplyCloneInternal(std::vector<CloneReferenceComponent>& referenceComponents, GameObject* source, const std::function<bool(GameObject*)>& predicate = 0);
        void ApplyClone(GameObject* source, const std::function<bool(GameObject*)>& predicate = 0);
        
    public:
        bool Recurse(const std::function<bool(const GameObject* object)>& function) const;
        bool HasComponent(ComponentId id) const override;
        void* GetComponent(ComponentId id) const override;
        void AddComponent(ComponentId id) override;
        void AddComponent(ComponentId id, GameObject* referenceObject) override;
        void RemoveComponent(ComponentId id) override;
        void CloneComponent(ComponentId id, GameObject* object) override;
        void ReplaceComponent(ComponentId id, GameObject* referenceObject) override;
        void EnableComponent(ComponentId id, bool enable) override;
        GameObject* GetComponentOwner(ComponentId id) const;
        int ComponentCount() const;
        
        template<typename T>
        bool HasComponent() const {
            return HasComponent(GameIdHelper::GetComponentID<T>());
        }
        
        template<typename T>
        T* GetComponent() const {
            return static_cast<T*>(GetComponent(GameIdHelper::GetComponentID<T>()));
        }
        
        template<typename T>
        T* AddComponent();
        
        template<typename T>
        T* AddComponent(GameObject* source);
        
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
        T* ReplaceComponent(GameObject* source);
        
        template<typename T>
        void EnableComponent(bool enable) {
            EnableComponent(GameIdHelper::GetComponentID<T>(), enable);
        }
        
        template<typename T>
        T* GetSystem();
        
        std::vector<TypeInfo> GetComponentTypes(const std::function<bool(int componentID)>& predicate = 0) const ;
        
        struct ComponentEditor {
            TypeInfo type;
            IFieldEditor* editor;
        };
        
        std::vector<ComponentEditor> GetComponentEditors(const std::function<bool(int componentID)>& predicate = 0) const;
        std::vector<int> GetComponentIndicies() const;
        
        InputManager& Input() const;
        
        void Remove() override;
        bool IsRemoved() const;
        
        GameObject* CreateChild();
        GameObject* CreateObject();
        GameObject* Root() const;
        GameObject* CreateChildClone(GameObject* source, const std::function<bool(GameObject*)>& predicate = 0);
        GameObject* CreateCopy(const std::function<bool(GameObject*)>& predicate = 0);
        
        bool IsRoot() const;
        
        int RootId() const;
        std::string& RootGuid() const;
        
        GameWorld* World() const;
        
        TypeInfo GetComponentTypeInfo(int index) const;
    
        template<typename T>
        GameObject* GetComponentOwner();
        
        void SetCallbacks(
       		const std::function<void(GameObject* object)>& ObjectCreated,
        	const std::function<void(GameObject* object)>& ObjectRemoved,
        	const std::function<void(GameObject* object, ComponentId componentId)>& ComponentCreated,
        	const std::function<void(GameObject* object, ComponentId componentId)>& ComponentRemoved
        ) const;
        
        bool HasAncestor(GameObject* ancestor) const;
        
        inline friend std::ostream& operator<<(std::ostream& stream, const GameObject& object) {
            stream<<object.RootGuid()<<":"<<object.id;
            return stream;
        }
        
        std::string TryGetScenePath() const;
        
        class Hierarchy& Hierarchy() const;
        
        static GameObject* Deserialize(const std::string& data);
    };
}
