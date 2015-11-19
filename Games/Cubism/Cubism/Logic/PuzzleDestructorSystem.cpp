//
//  PuzzleDestructorSystem.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 9/14/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PuzzleDestructorSystem.hpp"

void PuzzleDestructorSystem::AddedToWorld(Pocket::GameWorld &world) {
    patchDestructor = world.CreateSystem<PatchDestructor>();
}

void PuzzleDestructorSystem::Initialize() {
    AddComponent<Block>();
}

void PuzzleDestructorSystem::ObjectAdded(Pocket::GameObject *object) {
    
    
}

void PuzzleDestructorSystem::ObjectRemoved(Pocket::GameObject *object) {
    
    
}

void PuzzleDestructorSystem::DestroyPuzzle() {
    int counter = 0;
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        (*it)->Remove();
        (*it)->Children()[0]->Remove();
        counter++;
    }
    std::cout<<counter<<std::endl;
    patchDestructor->DestroyPatches();
}

void PuzzleDestructorSystem::PatchDestructor::Initialize() {
    AddComponent<Patch>();
}

void PuzzleDestructorSystem::PatchDestructor::DestroyPatches() {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        (*it)->Remove();
    }
}

