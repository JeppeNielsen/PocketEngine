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
        CalcViewportSize();
    });
    OSXWindowCreator::Instance()->Create();
}

void WindowOSX::osxWindowCreated(void* win) {
    this->win = win;
    
    OSXView* window = (OSXView*)win;

    NSRect windowBounds = [window bounds];
 
    GLsizei windowWidth  = (GLsizei)(windowBounds.size.width),
            windowHeight = (GLsizei)(windowBounds.size.height);
    
    context->ScreenSize = Vector2(windowWidth, windowHeight);
    OSXWindowCreator::Instance()->win = win;
    CalcViewportSize();
}

void WindowOSX::Loop(bool ee) {
    if (Step()) {
        exit(0);
    }
}

void WindowOSX::Destroy() {
	
}

bool WindowOSX::Update() {
    return true;
}

void WindowOSX::PreRender() {
    glViewport(0, 0, backingPixelWidth, backingPixelHeight);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
}

void WindowOSX::PostRender() {
    glFlush();
}

void WindowOSX::CalcViewportSize() {
    OSXView* window = (OSXView*)win;
    NSRect backingBounds = [window convertRectToBacking:window.bounds];
    backingPixelWidth  = (int)(backingBounds.size.width);
    backingPixelHeight = (int)(backingBounds.size.height);
}
