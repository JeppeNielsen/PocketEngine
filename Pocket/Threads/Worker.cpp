//
//  Worker.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 3/17/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Worker.hpp"
#include "Thread.hpp"

using namespace Pocket;

Worker::Worker()
:
Progress(this)
{
    thread = new Thread();
    thread->Code += event_handler(this, &Worker::Loop);
    Progress = 0;
}

Worker::~Worker() {
    delete thread;
}

void Worker::Start(Pocket::IDelegate<WorkerContext*>* workMethod) {
    Start(workMethod, 0);
}

void Worker::Start(Pocket::IDelegate<WorkerContext*>* workMethod, Pocket::IDelegate<WorkerContext*>* finishedMethod) {
    Work.Clear();
    Work += workMethod;
    if (finishedMethod) {
        JobDone.Clear();
        JobDone += finishedMethod;
    }
    thread->Start();
}

void Worker::Loop(Pocket::Thread *thread) {
    WorkerContext context;
    context.Worker = this;
    Work(&context);
    Thread::AddTask(new WorkerContext(context), event_handler(this, &Worker::Finished));
}

void Worker::SetProgress(float percent) {
    float* p = new float(percent);
    Thread::AddTask(p, event_handler(this, &Worker::SetComplete));
}

void Worker::SetComplete(void *p) {
    float* percent = (float*)p;
    Progress = *percent;
    delete percent;
}

void Worker::Finished(void *d) {
    thread->Stop();
    WorkerContext* context = (WorkerContext*)d;
    Progress = 1;
    JobDone(context);
    delete context;
}

WorkerContext::WorkerContext() {
    Data = 0;
}

WorkerContext::WorkerContext(const WorkerContext& other) {
    Worker = other.Worker;
    Data = other.Data;
}

void WorkerContext::SetProgress(float percent) {
    Worker->SetProgress(percent);
}

