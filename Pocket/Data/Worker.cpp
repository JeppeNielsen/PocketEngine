//
//  Worker.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 22/04/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Worker.hpp"

using namespace Pocket;

void Worker::Update() {
    for (int i=0; i<tasks.size(); ++i) {
        if (tasks[i].check()) {
            tasks[i].finished();
            tasks.erase(tasks.begin() + i);
            i--;
        }
    }
}
