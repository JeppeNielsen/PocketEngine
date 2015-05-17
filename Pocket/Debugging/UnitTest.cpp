//
//  UnitTest.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 9/30/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "UnitTest.hpp"
#include <iostream>

UnitTest::UnitTest(std::string name, UnitTest::Method method) {
    this->name = name;
    this->method = method;
    Run();
}

UnitTest::~UnitTest() { }

void UnitTest::Run() {
    bool succes = method();
    if (succes) {
        std::cout << name << ": SUCCES!"<< std::endl;
    } else {
        std::cout << name << ": FAILED!"<< std::endl;
    }
}
