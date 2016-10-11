//
//  GameSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>

/*namespace Pocket {
class GameObject;
}*/


namespace Pocket {
class GameObject {
private:
    virtual void* GetComponent(int componentID) = 0;
    virtual void AddComponent(int componentID) = 0;
    virtual void AddComponent(int componentID, GameObject* referenceObject) = 0;
    virtual void RemoveComponent(int componentID) = 0;
    virtual void CloneComponent(int componentID, GameObject* source) = 0;
public:
    template<typename T> T* GetComponent() { return (T*)0; }
    template<typename T> T* AddComponent() { }
    template<typename T> void RemoveComponent() { }
    template<typename T> T* CloneComponent(GameObject* source) { }
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
    virtual int Order() = 0;
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
    
    virtual int Order() override { return 0; }
    
private:
    using ObjectCollection = std::vector<GameObject*>;
    ObjectCollection objects;
    friend class GameObject;
protected:
    const ObjectCollection& Objects() {
        return objects;
    }
};

}