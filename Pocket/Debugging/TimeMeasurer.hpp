//
//  TimeMeasurer.h
//  Testing
//
//  Created by Jeppe Nielsen on 29/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include <functional>
#include "Timer.hpp"

namespace Pocket {
  class TimeMeasurer {
  public:
    typedef std::function<void()> Method;
    
    void AddTest(std::string name, Method method);
    void Run();
    
  private:
    struct Test {
        std::string name;
        Method method;
    };
    typedef std::vector<Test> Tests;
    Tests tests;
    
    Timer timer;
  };
}