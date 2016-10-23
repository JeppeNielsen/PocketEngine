//
//  UnitTests.cpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 11/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "LogicTests.hpp"
#include <cstdlib>
#include "GameWorld.hpp"
#include "GameSystem.hpp"

using namespace Pocket;


void LogicTests::RunTests() {
    
    AddTest("Create World", [] {
        GameWorld world;
        return world.Roots().size() == 0;
    });

    AddTest("Create Root", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        return root && world.Roots().size() == 1;
    });
    
    AddTest("Delete root", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        bool wasOne = world.Roots().size() == 1;
        root->Remove();
        world.Update(0);
        return wasOne && world.Roots().size() == 0;
    });
    
    AddTest("Create Child", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        GameObject* child = root->CreateChild();
        return root && child && root->Children().size() == 1;
    });

    AddTest("Delete Child", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        GameObject* child = root->CreateChild();
        bool wasOne = root->Children().size() == 1;
        child->Remove();
        world.Update(0);
        return wasOne && root->Children().size() == 0;
    });

    AddTest("Object Handle operator->, ", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        Handle<GameObject> handle = root;
        return handle.operator->() == root;
    });

    AddTest("Handle invalid on removed object ", [] {
        GameWorld world;
        GameObject* root = world.CreateRoot();
        Handle<GameObject> handle = root;
        bool wasValid = handle ? true : false;
        root->Remove();
        world.Update(0);
        return wasValid && !handle;
    });

}
