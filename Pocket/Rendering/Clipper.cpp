//
//  Clipper.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 10/13/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "Clipper.hpp"
#include "OpenGL.hpp"

using namespace Pocket;

Clipper::Clipper() { stackCount = 0; UseDepth = false; }

Clipper::~Clipper() {}

void Clipper::PushBegin() {
    
    if (stackCount==0) {
        glEnable(GL_STENCIL_TEST);
    }
    
    stackCount++;
    
    glClear(GL_STENCIL_BUFFER_BIT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_ALWAYS, stackCount, stackCount);
    glStencilOp(GL_INCR, GL_INCR, GL_INCR);
        
}

void Clipper::PushEnd() {
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(UseDepth ? GL_TRUE : GL_FALSE);
    glStencilFunc(GL_EQUAL, stackCount, stackCount);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
}

void Clipper::PopBegin() {
    if (stackCount == 0) return;
    
    stackCount--;
    
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glStencilFunc(GL_ALWAYS, stackCount, stackCount);
    glStencilOp(GL_DECR, GL_DECR, GL_DECR);
}


void Clipper::PopEnd() {
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(UseDepth ? GL_TRUE : GL_FALSE);
    
    if (stackCount==0) {
        glDisable(GL_STENCIL_TEST);
    }
}



