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

int main() {

    GameWorld world;
    
    auto root = world.CreateRoot();
    auto cube = root->CreateChild();
    cube->AddComponent<Transform>();
    
    auto transform = cube->GetComponent<Transform>();
    transform->Position = { 1,1,1 };
    
    auto animationObject = root->CreateObject();
    auto animation = animationObject->AddComponent<Animation>();
    
    animation->AddNode<Transform>(cube, "Position", 0.0f);
    transform->Position = { 2,2,2 };
    animation->AddNode<Transform>(cube, "Position", 1.0f);
    transform->Position = { 3,3,3 };
    animation->AddNode<Transform>(cube, "Position", 2.0f);
    
    
    transform->Matrix = Matrix4x4::CreateScale(2);
    animation->AddNode<Transform>(cube, "Matrix", 0.0f);
    
    transform->Matrix = Matrix4x4::CreateScale(4);
    animation->AddNode<Transform>(cube, "Matrix", 1.0f);
    

    //animationObject->ToJson(std::cout);
    
    std::cout <<" initial : " << std::endl;
    cube->ToJson(std::cout);
    
    
    animation->Apply(0.0f);
    std::cout <<" time = 0 : " << std::endl;
    cube->ToJson(std::cout);
    
    animation->Apply(0.5f);
    std::cout <<" time = 0.5 : " << std::endl;
    cube->ToJson(std::cout);
    
    
    animation->Apply(1.0f);
    std::cout <<" time = 1.0 : " << std::endl;
    cube->ToJson(std::cout);
    
    animation->Apply(1.5f);
    std::cout <<" time = 1.5 : " << std::endl;
    cube->ToJson(std::cout);
    
    animation->Apply(2.0f);
    std::cout <<" time = 2.0 : " << std::endl;
    cube->ToJson(std::cout);
    
    
     animation->Apply(100.0f);
    std::cout <<" time = 100.0 : " << std::endl;
    cube->ToJson(std::cout);

    return 0;
}
