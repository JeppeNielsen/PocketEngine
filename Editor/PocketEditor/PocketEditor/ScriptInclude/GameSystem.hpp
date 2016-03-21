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

class IScriptSystem {
public:
    virtual ~IScriptSystem() {}
    virtual void ObjectAdded(GameObject* object) = 0;
    virtual void ObjectRemoved(GameObject* object) = 0;
    virtual void Update(float dt) = 0;
    virtual int AddObject(GameObject* object) = 0;
    virtual GameObject* RemoveObject(int indexToRemove) = 0;
};

template<typename... T>
class GameSystem : public IScriptSystem {
protected:
    virtual void ObjectAdded(GameObject* object) override { }
    virtual void ObjectRemoved(GameObject* object) override { }
    virtual void Update(float dt) override {}
    
    virtual int AddObject(GameObject* object) override {
        int ret = (int)objects.size();
        objects.push_back(object);
        return ret;
    }
    
    virtual GameObject* RemoveObject(int indexToRemove) override {
        GameObject* lastObject = objects[objects.size()-1];
        objects[indexToRemove] = lastObject;
        objects.pop_back();
        return lastObject;
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