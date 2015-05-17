//
//  Worker.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/17/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Event.hpp"
#include "Property.hpp"

namespace Pocket {
    class Thread;
    class Worker;
    struct WorkerContext {
        WorkerContext();
        WorkerContext(const WorkerContext& other);
        Worker* Worker;
        void SetProgress(float percent);
        void* Data;
    };
    
    class Worker {
    public:
        Worker();
        ~Worker();
        
        void Start(Pocket::IDelegate<WorkerContext*>* workMethod);
        void Start(Pocket::IDelegate<WorkerContext*>* workMethod, Pocket::IDelegate<WorkerContext*>* finishedMethod);
        
        Event<WorkerContext*> JobDone;
        Property<Worker*, float> Progress;
                
    private:
        Thread* thread;
        
        void Loop(Thread* thread);
        void SetComplete(void* p);
        void Finished(void* d);
        
        Event<WorkerContext*> Work;
        
        void SetProgress(float percent);
        
        friend class WorkerContext;
    };
}