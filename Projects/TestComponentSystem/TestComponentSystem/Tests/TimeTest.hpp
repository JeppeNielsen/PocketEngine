//
//  TimeTest.hpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 12/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <functional>
#include <string>
#include <vector>

class TimeTest {
private:
    using Method = std::function<void()>;
    
    struct Test {
        std::string name;
        Method test;
    };
    
    using Tests = std::vector<Test>;
    Tests tests;
public:
    virtual ~TimeTest();
    void Run();
protected:
    void AddTest(const std::string& name, Method test);
    
    void Begin();
    void End();
    
    virtual void RunTests() = 0;
};