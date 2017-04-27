//
//  testWorker.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 22/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include <cmath>
#include "Worker.hpp"

using namespace Pocket;

int main_workerTest() {
    
    Worker worker;

    worker.DoTask([] () {
        double result = 0;
        for (int i=0; i<1000000000; i++) {
            result+=sqrt(i * 2);
        }
        return result;
    }, [] (const double& value) {
        std::cout << "Value calculated :"<<value<<std::endl;
    });
    
    worker.DoTask([] () {
        double result = 0;
        for (int i=0; i<1000000000; i++) {
            result-=sqrt(i * 2);
        }
        return result;
    }, [] (const double& value) {
        std::cout << "Value negative calculated :"<<value<<std::endl;
    });
    
    
    int counter = 0;
    while (true) {
        std::cout << "main thread "<<counter<<std::endl;
        std::this_thread::sleep_for(std::chrono::nanoseconds(100000000));
        counter++;
        worker.Update();
    }
    
        

    return 0;
}
