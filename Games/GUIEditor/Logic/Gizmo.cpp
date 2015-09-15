//
//  Gizmo.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 14/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include "Gizmo.h"


void Gizmo::Reset() {
    for (int i=0; i<8; ++i) {
        corners[i] = 0;
    }
}

