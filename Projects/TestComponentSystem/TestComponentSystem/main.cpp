//
//  main.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 07/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "LogicTests.hpp"
#include "ScriptTests.hpp"

int main(int argc, const char * argv[]) {

    LogicTests tests;
    tests.Run();
    ScriptTests scriptTests;
    scriptTests.Run();
    
    return 0;
}
