//
//  Thread.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/17/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Thread.hpp"
#include <pthread.h>
#include <unistd.h>
#include <map>

using namespace Pocket;

static pthread_mutex_t mutex;
static pthread_mutex_t task_mutex;
static pthread_mutex_t context_mutex;
static int numberOfThreads = 0;

static void* Method(void* t) {
    Thread::ThreadContext* threadContext = (Thread::ThreadContext*)t;
    threadContext->Code->operator()(threadContext->Thread);
    
    pthread_mutex_lock(&context_mutex);
    delete threadContext->Code;
    if (threadContext->Thread) {
        threadContext->Thread->Stop();
        threadContext->Thread->threadContext = 0;
    }
    delete threadContext;
    pthread_mutex_unlock(&context_mutex);
    return 0;
}

typedef std::vector<Thread::Task* > Tasks;

static Tasks tasks;
static Tasks taskToExecute;

Thread::Thread() {
    if (numberOfThreads == 0) {
        if (pthread_mutex_init(&mutex, NULL)!=0 ||
            pthread_mutex_init(&task_mutex, NULL)!=0 ||
            pthread_mutex_init(&context_mutex, NULL)!=0
            ) {
            printf("Mutexes initialization failed");
        } 
    }
    numberOfThreads++;
    running = false;
    threadContext = 0;
}

Thread::~Thread() {
    Stop();
    pthread_mutex_lock(&context_mutex);
    if (threadContext) {
        threadContext->Thread = 0;
        threadContext = 0;
    }
    pthread_mutex_unlock(&context_mutex);
    numberOfThreads--;
    if (numberOfThreads==0) {
        pthread_mutex_destroy(&mutex);
        pthread_mutex_destroy(&task_mutex);
        pthread_mutex_destroy(&context_mutex);
        
        for (unsigned int i=0; i<tasks.size(); i++) {
            delete tasks[i];
        }
        tasks.clear();
    }    
}

void Thread::Start() {
    if (running) return;
    threadContext = new ThreadContext();
    threadContext->Thread = this;
    threadContext->Code = new Event<Pocket::Thread*>(Code);
    pthread_t* t = new pthread_t();
    pthread_create(t, NULL, Method, threadContext);
    running = true;
}

void Thread::Stop() {
    if (!running) return;
    running = false;
}

bool Thread::Running() {
    return running;
}

void Thread::Lock() {
    pthread_mutex_lock(&mutex);
}

void Thread::Unlock() {
    pthread_mutex_unlock(&mutex);
}

void Thread::Sleep(unsigned int microseconds) {
    usleep(microseconds);
}

void Thread::AddTask(void* data, std::function<void(void*)>& method) {
    Task* task = new Task();
    task->data = data;
    task->event.Bind(method);
    pthread_mutex_lock(&task_mutex);
    tasks.push_back(task);
    pthread_mutex_unlock(&task_mutex);
}

void Thread::Update() {
    if (numberOfThreads == 0) return;
    //return;
    
    pthread_mutex_lock(&task_mutex);
    for (unsigned int i=0; i<tasks.size(); i++) {
        taskToExecute.push_back(tasks[i]);
    }
    tasks.clear();
    pthread_mutex_unlock(&task_mutex);

    for (unsigned int i=0; i<taskToExecute.size(); i++) {
        taskToExecute[i]->event(taskToExecute[i]->data);
        delete taskToExecute[i];
    }
    taskToExecute.clear();
}

