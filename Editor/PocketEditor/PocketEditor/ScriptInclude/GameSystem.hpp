//
//  GameSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "Property.hpp"

/*namespace Pocket {
class GameObject;
}*/

namespace Pocket {
class GameObject {
private:

    virtual bool HasComponent(int id) const = 0;
    virtual void* GetComponent(int id) const = 0;
    virtual void AddComponent(int id) = 0;
    virtual void AddComponent(int id, GameObject* referenceObject) = 0;
    virtual void RemoveComponent(int id) = 0;
    virtual void CloneComponent(int id, GameObject* object) = 0;
    virtual void ReplaceComponent(int id, GameObject* object) = 0;
    virtual void EnableComponent(int id, bool enable) = 0;
    
public:
    template<typename T> T* GetComponent() { return (T*)0; }
    template<typename T> T* AddComponent() { }
    template<typename T> void RemoveComponent() { }
    template<typename T> T* CloneComponent(GameObject* source) { }
    template<typename T> T* ReplaceComponent(GameObject* source) { }
    template<typename T> void EnableComponent(bool enable) { }
};
}


struct IGameSystem {
    virtual ~IGameSystem() = default;
    virtual void Initialize() = 0;
    virtual void Destroy() = 0;
    virtual void ObjectAdded(Pocket::GameObject* object) = 0;
    virtual void ObjectRemoved(Pocket::GameObject* object) = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual int AddObject(Pocket::GameObject* object) = 0;
    virtual void RemoveObject(Pocket::GameObject* object) = 0;
    virtual int ObjectCount() = 0;
    virtual int GetOrder() = 0;
    virtual void SetOrder(int order) = 0;
    virtual int GetIndex() = 0;
    virtual void SetIndex(int index) = 0;
};

namespace Pocket {

template<typename... T>
class GameSystem : public IGameSystem {
protected:
    virtual ~GameSystem() { }
    virtual void Initialize() override { }
    virtual void Destroy() override { }
    virtual void ObjectAdded(GameObject* object) override { }
    virtual void ObjectRemoved(GameObject* object) override { }
    virtual void Update(float dt) override {}
    virtual void Render() override {}
    
    int AddObject(GameObject* object) override {
        int ret = (int)objects.size();
        objects.push_back(object);
        return ret;
    }
    
    void RemoveObject(GameObject* object) override {
        objects.erase(std::find(objects.begin(), objects.end(), object));
//        GameObject* lastObject = objects[objects.size()-1];
//        objects[indexToRemove] = lastObject;
//        objects.pop_back();
//        //return lastObject;
    }
    
    int ObjectCount() override {
        return (int)objects.size();
    }
    
    Pocket::Property<int> Order;
    
    int GetOrder() override { return Order(); }
    void SetOrder(int order) override { Order = order; }
    int GetIndex() override { return index; }
    void SetIndex(int index) override { this->index = index; }
    
private:
    using ObjectCollection = std::vector<GameObject*>;
    ObjectCollection objects;
    friend class GameObject;
    int index;
protected:
    const ObjectCollection& Objects() {
        return objects;
    }
};

}
