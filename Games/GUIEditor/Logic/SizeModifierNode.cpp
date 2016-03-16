//
//  SizeModifierNode.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/22/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SizeModifierNode.hpp"

SizeModifierNode::SizeModifierNode() {
    cornerIndex = 0;
    transformTarget = 0;
    sizableTarget = 0;
}

Vector2 SizeModifierNode::GetCornerPosition() {
    const Vector2 cornerPositions[] = {
        {0,0}, {0,0.5f},{0,1},{0.5f,1},
        {1,1}, {1,0.5f},{1,0},{0.5f,0}
    };
    return cornerPositions[cornerIndex];
}