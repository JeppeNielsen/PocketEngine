//
//  WorkScheduler.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/17/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include "Event.hpp"
#include "Worker.hpp"
#include <vector>

namespace Pocket {
    class Worker;
    class WorkerContext;
    class WorkScheduler {
    public:
        WorkScheduler();
        ~WorkScheduler();
        
        void Start(Pocket::IDelegate<WorkerContext*>* workMethod,
                   Pocket::IDelegate<WorkerContext*>* finishedMethod);
        
        void Start(Pocket::IDelegate<WorkerContext*>* workMethod,
                   Pocket::IDelegate<WorkerContext*>* finishedMethod,
                   Pocket::IDelegate<Worker*>* progressChanged);
        
    private:
        typedef std::vector<Worker*> Workers;
        Workers workers;
        
        void WorkerFinished(WorkerContext* context);
        
    };
}