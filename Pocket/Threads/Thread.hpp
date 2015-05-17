//
//  Thread.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/17/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Event.hpp"
#include <vector>

namespace Pocket {
   
    class Engine;
    class Thread {
      
    public:
        Thread();
        ~Thread();
        
        void Start();
        void Stop();
        
        Event<Thread*> Code;
        
        bool Running();
        
        static void Lock();
        static void Unlock();
        static void Sleep(unsigned int microseconds);
                
        struct Task {
            void* data;
            Event<void*> event;
        };
        
        static void AddTask(void* data, Pocket::IDelegate<void*>* method);
        
        struct ThreadContext {
            Thread* Thread;
            Event<Pocket::Thread*>* Code;
        };
        
        ThreadContext* threadContext;
        
        friend class Engine;
    private:
        static void Update();
        bool running;
        
    };
}