//
//  UnitTest.hpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 11/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include <functional>
#include <string>
#include <vector>

class LogicTest {
private:
    using TestMethod = std::function<bool()>;

    struct Test {
        std::string name;
        TestMethod method;
    };
    
    using Tests = std::vector<Test>;
    Tests tests;
public:
    void Run();
    virtual ~LogicTest();
protected:
    void AddTest(const std::string& name, TestMethod test);
    virtual void RunTests() = 0;
};