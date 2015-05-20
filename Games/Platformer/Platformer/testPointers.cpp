//
//  testPointers.cpp
//  Platformer
//
//  Created by Jeppe Nielsen on 09/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "GameWorld.hpp"
#include "IPointable.hpp"

using namespace std;
using namespace Nano;




Component(Bone)
public:
    std::string name;
};

Component(Model)
public:
    Pointer<GameObject> gameObject;
    Pointer<Bone> bone;
};

GameObject* createModel(GameWorld& world, std::string boneName) {
    GameObject* modelObject = world.CreateObject();
    Model* model = modelObject->AddComponent<Model>();
    GameObject* boneObject = world.CreateObject();
    Bone* bone = boneObject->AddComponent<Bone>();
    bone->name = boneName;
    model->gameObject = boneObject;
    model->bone = bone;
    boneObject->Parent = modelObject;
    return modelObject;
}

int main_nono() {

/*
    Pointer<GameObject> pointer;
    {
        GameWorld world;
    
        GameObject* bone = world.CreateObject();
    
        GameObject* go = world.CreateObject();
        go->AddComponent<Model>()->bone = bone;
        pointer = go;
        GameObject* clone = go->Clone();
        
        
        
    }
    
    return 0;
    */
    GameWorld world;
    GameObject* model = createModel(world, "First");
    GameObject* clone = model->Clone();
    model->GetComponent<Model>()->gameObject->GetComponent<Bone>()->name = "Changed";
    std::cout<<model->GetComponent<Model>()->gameObject->GetComponent<Bone>()->name << std::endl;
    std::cout<<model->GetComponent<Model>()->bone->name<<std::endl;
    std::cout<<clone->GetComponent<Model>()->gameObject->GetComponent<Bone>()->name << std::endl;
    std::cout<<clone->GetComponent<Model>()->bone->name<<std::endl;
    
    
    world.Update(1.0f);
    
    return 0;
}




/*

class GameObject : public IPointable<GameObject> {
    
    

};

class Collection {
public:
    GameObject obj;
};


int main() {

    GameObject* ob = new GameObject();
    
    Pointer<GameObject> pointer;
    pointer = ob;
    
    delete ob;
    
    

    Collection b;

    {
        Pointer<GameObject> pointer1;
        {
            Collection a;
            pointer1 = &a.obj;
            b = a;
            std::cout<<a.obj.pointers.size()<<std::endl;
        }
        std::cout<<(pointer1 ? " valid" : "invalid")<<std::endl;
    }
    
{

}

}

*/

