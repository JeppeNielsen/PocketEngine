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
    OSXWindowCreator::Instance()->ScreenSizeChanged.Bind([this](int w, int h){
        std::cout << "Screen size changed x:"<<w<< " : " << h << std::endl;
        context->ScreenSize = Vector2(w, h);
    });
    
    OSXWindowCreator::Instance()->Create();
    
}

void WindowOSX::osxWindowCreated(void* win) {
    
    OSXView* window = (OSXView*)win;
    
    
    NSRect backingBounds = [window bounds];//;[window convertRectToBacking:window.bounds];
 
    GLsizei backingPixelWidth  = (GLsizei)(backingBounds.size.width),
            backingPixelHeight = (GLsizei)(backingBounds.size.height);
    
    
    context->ScreenSize = Vector2(backingPixelWidth, backingPixelHeight);
    OSXWindowCreator::Instance()->win = win;
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
    const Vector2 screenSize = context->ScreenSize;
    glViewport(0, 0, screenSize.x, screenSize.y);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);    
}

void WindowOSX::PostRender() {
    //glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer);
	//[((EAGLContext*)context) presentRenderbuffer:GL_RENDERBUFFER];
    glFlush();
}

