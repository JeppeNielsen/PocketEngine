//
//  main.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 07/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include "LogicTests.hpp"
//#include "PerformanceTests.hpp"
//#include "GameWorld.hpp"

int main(int argc, const char * argv[]) {
    
//    std::cout << sizeof(Pocket::GameObject)<<std::endl;
//    std::cout << sizeof(Pocket::GameWorld)<<std::endl;
    
    LogicTests tests;
    tests.Run();
    //PerformanceTests tests;
    //tests.Run();
    /*
    for(int i = 0; i<100000; ++i) {
        Pocket::GameWorld world;
        for(int j = 0; j<10; ++j) {
            world.CreateObject();
        }
    }
    */
    
    return 0;
}
