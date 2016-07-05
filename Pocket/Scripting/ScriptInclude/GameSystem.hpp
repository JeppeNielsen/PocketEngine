//
//  GameSystem.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>

class GameObject;

struct IGameSystem {
    virtual ~IGameSystem() = default;
    virtual void Initialize()=0;
    virtual void ObjectAdded(GameObject* object) = 0;
    virtual void ObjectRemoved(GameObject* object) = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual int AddObject(GameObject* object) = 0;
    virtual void RemoveObject(GameObject* object) = 0;
};

template<typename... T>
class GameSystem : public IGameSystem {
protected:
    virtual ~GameSystem() { }
    virtual void Initialize() override { }
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
    
private:
    using ObjectCollection = std::vector<GameObject*>;
    ObjectCollection objects;
    friend class GameObject;
protected:
    const ObjectCollection& Objects() {
        return objects;
    }
};