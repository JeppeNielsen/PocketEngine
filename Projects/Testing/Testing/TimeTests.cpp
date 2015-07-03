//
//  TimeTests.cpp
//  Testing
//
//  Created by Jeppe Nielsen on 29/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "TimeTests.hpp"
#include "GameWorld.hpp"
#include "TimeMeasurer.hpp"
#include "Transform.hpp"
#include "VelocitySystem.hpp"
#include "TransformAnimatorSystem.hpp"

using namespace Pocket;

const int NumberOfObjects = 1000000;

void TimeTests::Run() {

    GameWorld world;

    world.CreateSystem<VelocitySystem>();
    world.CreateSystem<TransformAnimatorSystem>();
    
    TimeMeasurer measurer;
    
    measurer.AddTest("Create 1 million objects", [&]() {
        //GameWorld world;
        for (int i=0; i<NumberOfObjects; ++i) {
            world.CreateObject();
        }
    });
    
    measurer.AddTest("remove 1 million objects", [&]() {
        for (int i=0; i<world.Objects().size(); ++i) {
            world.Objects()[i]->Remove();
        }
        world.Update(0.01f);
    });
    
    measurer.AddTest("Create 1 million objects with Transform and Velocity components", [&]() {
        for (int i=0; i<NumberOfObjects; ++i) {
            GameObject* object = world.CreateObject();
            object->AddComponent<Transform>();
            object->AddComponent<Velocity>();
        }
    });
    
    measurer.AddTest("Update 1 million objects", [&] () {
        world.Update(1.0f);
    });
    
    measurer.Run();
}