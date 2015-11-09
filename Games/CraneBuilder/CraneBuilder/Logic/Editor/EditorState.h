//
//  EditorState.h
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 09/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"

using namespace Pocket;

Component(EditorState)

    EditorState();
    void Reset();

    enum class State {
        None,
        Terrain,
    };

    Property<EditorState*, State> CurrentState;
};