//
//  CameraMover.cpp
//  CraneBuilder
//
//  Created by Jeppe Nielsen on 07/11/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "CameraDragger.h"

void CameraDragger::Reset() {
    startDragPosition = {0,0,0};
    startTransformPosition = {0,0,0};
    isDragging = false;
    zPos = 20.0f;
}