//
//  EditorState.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 09/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "EditorState.h"

EditorState::EditorState() : CurrentState(this) {}

void EditorState::Reset() {
    CurrentState = State::None;
}