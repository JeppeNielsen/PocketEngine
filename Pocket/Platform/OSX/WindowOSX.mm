//
//  WindowOSX.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/6/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "WindowOSX.hpp"
#include "OpenGL.hpp"
#include "OSXWindowCreator.h"
#include <iostream>
#include <map>
#include "Vector2.hpp"
#include "OSXView.h"

using namespace std;
using namespace Pocket;


void WindowOSX::Create(int width, int height, bool fullScreen) {
    OSXWindowCreator::Instance()->Width = width;
    OSXWindowCreator::Instance()->Height = height;
    OSXWindowCreator::Instance()->FullScreen = fullScreen;
}

void WindowOSX::Begin() {
    
    OSXWindowCreator::Instance()->OnInitialize.Bind(this, &WindowOSX::osxWindowCreated);
    OSXWindowCreator::Instance()->OnUpdate.Bind(this, &WindowOSX::Loop);
    OSXWindowCreator::Instance()->Create();
    
}

void WindowOSX::osxWindowCreated(void* win) {
    
    OSXView* window = (OSXView*)win;
    
    screenSize = Vector2(window.bounds.size.width, window.bounds.size.height);
    
}

void WindowOSX::Loop(bool ee) {
    if (Step()) {
        exit(0);
    }
}

void WindowOSX::Destroy() {
	
}

bool WindowOSX::Update(IInputManagerIterator* inputManagers) {
    OSXWindowCreator::Instance()->inputDevice.Update(inputManagers);
	return true;
}

void WindowOSX::PreRender() {
    //[EAGLContext setCurrentContext:((EAGLContext*)context)];
	//glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer);
    glViewport(0, 0, screenSize.x, screenSize.y);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);    
}

void WindowOSX::PostRender() {
    //glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
	//[((EAGLContext*)context) presentRenderbuffer:GL_RENDERBUFFER];
    glFlush();
}

