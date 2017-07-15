//
//  testAnimation.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 02/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//





#include "Animation.hpp"
#include "Transform.hpp"
#include "GameWorld.hpp"
#include "Mesh.hpp"
#include <iostream>

using namespace Pocket;

int main_Animation() {

    GameWorld world;
    
    auto root = world.CreateRoot();
    
    auto animationObject = root->CreateObject();
    auto animation = animationObject->AddComponent<Animation>();
    
    auto cube = root->CreateChild();
    {
        cube->AddComponent<Transform>();
        
        auto transform = cube->GetComponent<Transform>();
        transform->Position = { 1,1,1 };
        animation->AddNode<Transform>(cube, cube, "Position", 0.0f);
        transform->Position = { 2,2,2 };
        animation->AddNode<Transform>(cube, cube, "Position", 1.0f);
        transform->Position = { 3,3,3 };
        animation->AddNode<Transform>(cube, cube, "Position", 2.0f);
        
        
        transform->Matrix = Matrix4x4::CreateScale(2);
        animation->AddNode<Transform>(cube, cube, "Matrix", 0.0f);
        
        transform->Matrix = Matrix4x4::CreateScale(4);
        animation->AddNode<Transform>(cube, cube, "Matrix", 1.0f);
        
        Quaternion q;
        q.FromEuler({0,0,0});
        transform->Rotation = q;
        animation->AddNode<Transform>(cube, cube, "Rotation", 0.0f);
        
        Quaternion q2;
        q2.FromEuler({0,60,0});
        transform->Rotation = q2;
        animation->AddNode<Transform>(cube, cube, "Rotation", 1.0f);
        
        
    }
        GameObject* child = cube->CreateChild();
        auto tr = child->AddComponent<Transform>();
        tr->Scale = {1,1,1};
        animation->AddNode<Transform>(cube, child, "Scale", 0.0f);
        tr->Scale = {2,2,2};
        animation->AddNode<Transform>(cube, child, "Scale", 1.0f);
    

    GameObject* clonedAnimationObject = root->CreateChildClone(animationObject);
    
    tr->Scale = {3,3,3};
    animation->AddNode<Transform>(cube, child, "Scale", 2.0f);

    clonedAnimationObject->ToJson(std::cout);
    
    animationObject->ToJson(std::cout);

    std::stringstream s;
    animationObject->ToJson(s);

    GameObject* newAnimObject = root->CreateChildFromJson(s);
    
    std::cout <<"--------------__--------------------________-" << std::endl;
    newAnimObject->ToJson(std::cout);
    
    animation = newAnimObject->GetComponent<Animation>();
    
    std::cout <<" initial : " << std::endl;
    cube->ToJson(std::cout);
    
    animation->Apply(cube, 0.0f);
    std::cout <<" time = 0 : " << std::endl;
    cube->ToJson(std::cout);
    
    animation->Apply(cube, 0.5f);
    std::cout <<" time = 0.5 : " << std::endl;
    cube->ToJson(std::cout);
    
    
    animation->Apply(cube, 1.0f);
    std::cout <<" time = 1.0 : " << std::endl;
    cube->ToJson(std::cout);
    
    animation->Apply(cube, 1.5f);
    std::cout <<" time = 1.5 : " << std::endl;
    cube->ToJson(std::cout);
    
    animation->Apply(cube, 2.0f);
    std::cout <<" time = 2.0 : " << std::endl;
    cube->ToJson(std::cout);
    
    
    animation->Apply(cube, 100.0f);
    std::cout <<" time = 100.0 : " << std::endl;
    cube->ToJson(std::cout);

    return 0;
}
