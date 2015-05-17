//
//  UnitTest.h
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 9/30/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>

class UnitTest {
public:
    typedef bool (* Method)();
    
    UnitTest(std::string name, Method method);
    ~UnitTest();
    
    void Run();
    
private:
    std::string name;
    Method method;
    
};

