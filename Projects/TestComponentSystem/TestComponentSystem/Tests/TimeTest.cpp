//
//  TimeTest.cpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 12/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "TimeTest.hpp"
#include "Timer.hpp"
#include <iostream>

TimeTest::~TimeTest() {}

static Pocket::Timer timer;
static double timerTime;

void TimeTest::Run() {
    tests.clear();
    RunTests();
    
    for(auto& test : tests) {
        test.test();
        std::cout<<test.name << " -> "<< timerTime<<" seconds:" <<std::endl;
    }
}

void TimeTest::AddTest(const std::string &name, Method test) {
    tests.push_back({name, test});
}

void TimeTest::Begin() {
    timer.Begin();
}

void TimeTest::End() {
    timerTime = timer.End();
}