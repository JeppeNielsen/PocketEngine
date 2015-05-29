//
//  TimeMeasurer.cpp
//  Testing
//
//  Created by Jeppe Nielsen on 29/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "TimeMeasurer.hpp"
#include <iostream>

using namespace Pocket;

void TimeMeasurer::AddTest(std::string name, Method method) {
    tests.push_back({name, method });
}

void TimeMeasurer::Run() {
    for(auto& test : tests) {
        
        timer.Begin();
        test.method();
        double time = timer.End();
        std::cout<<test.name<<" Time : "<< time<<" seconds"<< std::endl;
    }
}
