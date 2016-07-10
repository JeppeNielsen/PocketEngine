//
//  GameObject.hpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 06/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameIDHelper.hpp"
#include "Property.hpp"
#include "DirtyProperty.hpp"
#include "TypeInfo.hpp"

namespace Pocket {
    
    class ScriptWorld;
    
    template<typename T>
    class Container;
    
    class GameObject;
    
    using ObjectCollection = std::vector<GameObject*>;
    
    class GameWorld;
    
    struct IGameObject {
        virtual void* GetComponent(ComponentID id) = 0;
        virtual void AddComponent(ComponentID id) = 0;
        virtual void AddComponent(ComponentID id, GameObject* referenceObject) = 0;
        virtual void RemoveComponent(ComponentID id) = 0;
        virtual void CloneComponent(ComponentID id, GameObject* object) = 0;
    };
    
    class GameObject : public IGameObject {
    public:
    
        struct ComponentInfo {
            std::string name;
            std::function<TypeInfo(GameObject*)> getTypeInfo;
        };
    
        template<typename T>
        bool HasComponent() const {
            return HasComponent(GameIDHelper::GetComponentID<T>());
        }
    
        template<typename T>
        T* GetComponent();
        
        template<typename T>
        T* AddComponent() {
            ComponentID id = GameIDHelper::GetComponentID<T>();
            TryAddComponentContainer(id, [id](GameObject::ComponentInfo& componentInfo){
                componentInfo.name = GameIDHelper::GetClassName<T>();
                
                componentInfo.getTypeInfo = 0;
                T* ptr = 0;
                Meta::static_if<Meta::HasGetTypeFunction::apply<T>::value, T*>(ptr, [&componentInfo, id](auto p) {
                    using SerializedComponentType = typename std::remove_pointer<decltype(p)>::type;
                    
                    componentInfo.getTypeInfo = [](GameObject* object) -> TypeInfo {
                        auto component = object->GetComponent<SerializedComponentType>();
                        return component->GetType();
                    };
                });
                return new Container<T>();
            });
            AddComponent(id);
            return GetComponent<T>();
        }
        
        template<typename T>
        T* AddComponent(GameObject* source) {
            ComponentID id = GameIDHelper::GetComponentID<T>();
            TryAddComponentContainer(id, [id](GameObject::ComponentInfo& componentInfo){
                componentInfo.name = GameIDHelper::GetClassName<T>();
                
                componentInfo.getTypeInfo = 0;
                T* ptr = 0;
                Meta::static_if<Meta::HasGetTypeFunction::apply<T>::value, T*>(ptr, [&componentInfo, id](auto p) {
                    using SerializedComponentType = typename std::remove_pointer<decltype(p)>::type;
                    componentInfo.getTypeInfo = [](GameObject* object) -> TypeInfo {
                        auto component = object->GetComponent<SerializedComponentType>();
                        return component->GetType();
                    };
                });
                
                return new Container<T>();
            });
            AddComponent(id, source);
            return GetComponent<T>();
        }
        
        template<typename T>
        T* CloneComponent(GameObject* source) {
            CloneComponent(GameIDHelper::GetComponentID<T>(), source);
            return GetComponent<T>();
        }
        
        template<typename T>
        void RemoveComponent() {
            RemoveComponent(GameIDHelper::GetComponentID<T>());
        };
        
        void Remove();
        bool IsRemoved();
        
        GameObject* Clone();
        
        const ObjectCollection& Children() const;
        Property<GameObject*>& Parent();
        Property<bool>& Enabled();
        DirtyProperty<bool>& WorldEnabled();
        Property<int>& Order();
        
        std::vector<TypeInfo> GetComponentTypes(std::function<bool(int componentID)> predicate);
        
    private:
    
        GameObject();
        virtual ~GameObject();
        
        GameObject(GameObject&& o) = delete;
        GameObject(const GameObject& o) = delete;
        GameObject& operator=(const GameObject& o) = delete;
        
        bool HasComponent(ComponentID id) const;
        
    public:
        void* GetComponent(ComponentID id) override;
        void AddComponent(ComponentID id) override;
        void AddComponent(ComponentID id, GameObject* source) override;
        void CloneComponent(ComponentID id, GameObject* source) override;
        void RemoveComponent(ComponentID id) override;
    private:
        void TryAddComponentContainer(ComponentID id, std::function<IContainer*(GameObject::ComponentInfo&)>&& constructor);
        void SetWorldEnableDirty();
        void SetEnabled(bool enabled);
        void TrySetComponentEnabled(ComponentID id, bool enable);
        
        struct Data {
            bool removed;
            Bitset activeComponents;
            Bitset enabledComponents;
            Property<GameObject*> Parent;
            Property<bool> Enabled;
            DirtyProperty<bool> WorldEnabled;
            ObjectCollection children;
            Property<int> Order;
        };
        
        int index;
        GameWorld* world;
        Data* data;
        
        friend class Container<GameObject>;
        friend class GameWorld;
        friend class ScriptWorld;
    };
}