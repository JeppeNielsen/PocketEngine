//
//  SizeModifier.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/22/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "SizeModifier.hpp"
#include "SizeModifierNode.hpp"

void SizeModifier::Reset() {
    for (int i=0; i<8; i++) {
        Nodes[i] = 0;
    }
}

void SizeModifier::DeleteNodes() {
    for (int i=0; i<8; i++) {
        if (Nodes[i]) {
            Nodes[i]->Remove();
            Nodes[i] = 0;
        }
    }
}