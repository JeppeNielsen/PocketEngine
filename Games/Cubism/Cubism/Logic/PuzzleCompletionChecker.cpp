//
//  PuzzleCompletionChecker.cpp
//  Cubism
//
//  Created by Jeppe Nielsen on 9/14/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "PuzzleCompletionChecker.hpp"


void PuzzleCompletionChecker::Initialize() {
    AddComponent<Block>();
}

void PuzzleCompletionChecker::ObjectAdded(Pocket::GameObject *object) {
    object->GetComponent<Block>()->Patch.Changed += event_handler(this, &PuzzleCompletionChecker::BlockChanged);
    object->GetComponent<Block>()->Blocked.Changed += event_handler(this, &PuzzleCompletionChecker::BlockChanged);
    object->GetComponent<Block>()->Moving.Changed += event_handler(this, &PuzzleCompletionChecker::BlockChanged);
}

void PuzzleCompletionChecker::ObjectRemoved(Pocket::GameObject *object) {
    object->GetComponent<Block>()->Patch.Changed -= event_handler(this, &PuzzleCompletionChecker::BlockChanged);
    object->GetComponent<Block>()->Blocked.Changed -= event_handler(this, &PuzzleCompletionChecker::BlockChanged);
    object->GetComponent<Block>()->Moving.Changed -= event_handler(this, &PuzzleCompletionChecker::BlockChanged);
}

void PuzzleCompletionChecker::BlockChanged(Block *block) {
    CheckForCompletion();
}

void PuzzleCompletionChecker::CheckForCompletion() {
    for (ObjectCollection::const_iterator it = Objects().begin(); it!=Objects().end(); ++it) {
        Block* block = (*it)->GetComponent<Block>();
        if (block->Patch == 0) return;
        if (block->Blocked) return;
        if (block->Moving) return;
    }
    
    PuzzleComplete(true);
}