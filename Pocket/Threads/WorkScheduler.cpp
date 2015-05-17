//
//  WorkScheduler.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/17/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "WorkScheduler.hpp"

using namespace Pocket;

WorkScheduler::WorkScheduler() {
    
}

WorkScheduler::~WorkScheduler() {
    for (unsigned int i=0; i<workers.size(); i++) {
        delete workers[i];
    }
    workers.clear();
}


void WorkScheduler::Start(Pocket::IDelegate<WorkerContext *> *workMethod,
                          Pocket::IDelegate<WorkerContext *> *finishedMethod) {
    Start(workMethod, finishedMethod, 0);
}

void WorkScheduler::Start(Pocket::IDelegate<WorkerContext *> *workMethod,
                          Pocket::IDelegate<WorkerContext *> *finishedMethod,
                          Pocket::IDelegate<Worker*>* progressChanged) {
    Worker* worker = new Worker();
    workers.push_back(worker);
    worker->Start(workMethod, finishedMethod);
    worker->JobDone += event_handler(this, &WorkScheduler::WorkerFinished);
    if (progressChanged) worker->Progress.Changed += progressChanged;
}

void WorkScheduler::WorkerFinished(Pocket::WorkerContext *context) {
    workers.erase(find(workers.begin(), workers.end(), context->Worker));
    delete context->Worker;
}
