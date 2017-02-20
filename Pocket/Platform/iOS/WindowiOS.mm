//
//  IOSWindow.cpp
//  Pocket
//
//  Created by Jeppe Nielsen on 3/10/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "WindowiOS.hpp"
#include "OpenGL.hpp"
#include "IOSWindowCreator.h"
#include <iostream>
#include <map>
#include "Vector2.hpp"
#include "IOSWindow.h"
#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

using namespace std;
using namespace Pocket;


void WindowiOS::Create(int width, int height, bool landscape) {
    IOSWindowCreator::Instance()->isLandscape = landscape;
}

void WindowiOS::Begin() {

    context->InputDevice().Initialize(12);
    
    IOSWindowCreator::Instance()->inputDevice = &context->InputDevice();
    IOSWindowCreator::Instance()->OnInitialize.Bind(this, &WindowiOS::iosWindowCreated);
    IOSWindowCreator::Instance()->OnUpdate.Bind(this, &WindowiOS::Loop);
    IOSWindowCreator::Instance()->inputDevice->KeyboardChanged.Bind(this, &WindowiOS::KeyboardChanged);
    IOSWindowCreator::Instance()->Create();
}

void WindowiOS::iosWindowCreated(void* win) {
    
    window = win;
    
    /*
    UIWindow* window = (UIWindow*)win;
    
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    EAGLContext* _context = [[EAGLContext alloc] initWithAPI:api];
    if (!_context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context");
        exit(1);
    }
    
    if (![EAGLContext setCurrentContext:_context]) {
        NSLog(@"Failed to set current OpenGL context");
        exit(1);
    }
    
    context = _context;
    

    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(id<EAGLDrawable>)window.layer];
    
    
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, _colorRenderBuffer);
    
    screenSize = Vector2(window.bounds.size.width, window.bounds.size.height);
     
     */
    
    renderContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    
    UIWindow* window = (UIWindow*)win;
    
    [EAGLContext setCurrentContext:((EAGLContext*)renderContext)];
    
    ASSERT_GL(glGenFramebuffers(1, &viewFramebuffer));
	ASSERT_GL(glGenRenderbuffers(1, &viewRenderbuffer));
	
	ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer));
	ASSERT_GL(glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer));
	[((EAGLContext*)renderContext) renderbufferStorage:GL_RENDERBUFFER fromDrawable:(id<EAGLDrawable>)window.layer];
	ASSERT_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, viewRenderbuffer));
	
    
	ASSERT_GL(glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth));
	ASSERT_GL(glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight));
	
    ASSERT_GL(glGenRenderbuffers(1, &depthRenderbuffer));
    ASSERT_GL(glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer));
    ASSERT_GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, backingWidth, backingHeight));
    ASSERT_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer));
    
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		NSLog(@"failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
		
	}
    
    Window::Framebuffer = viewFramebuffer;
     
    context->ScreenSize = Vector2(window.bounds.size.width, window.bounds.size.height);
}

void WindowiOS::Loop(bool ee) {
    if (Step()) {
        exit(0);
    }
}

void WindowiOS::Destroy() {
	
}

bool WindowiOS::Update() {
	
    
	//inputDevice.SetTouchPosition(0, mousePosition);
	//inputDevice.SetTouchPosition(1, mousePosition);
	//inputDevice.SetTouchPosition(2, mousePosition);
    
	//inputDevice.SetTouch(0, active && ((GetKeyState(VK_LBUTTON) & 0x80) != 0), mousePosition);
	//inputDevice.SetTouch(1, active && ((GetKeyState(VK_RBUTTON) & 0x80) != 0), mousePosition);
	//inputDevice.SetTouch(2, active && ((GetKeyState(VK_MBUTTON) & 0x80) != 0), mousePosition);
    
    //IOSWindowCreator::Instance()->inputDevice.Update(inputManagers);
	return true;
}

void WindowiOS::PreRender() {
    const Vector2& screenSize = context->ScreenSize;
    [EAGLContext setCurrentContext:((EAGLContext*)renderContext)];
	ASSERT_GL(glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer));
    ASSERT_GL(glViewport(0, 0, screenSize.x, screenSize.y));
}

void WindowiOS::PostRender() {
    ASSERT_GL(glBindRenderbuffer(GL_RENDERBUFFER, viewRenderbuffer));
	[((EAGLContext*)renderContext) presentRenderbuffer:GL_RENDERBUFFER];
}

void WindowiOS::KeyboardChanged(InputDevice::KeyboardEventData keyboardData) {
    IOSWindow* win = (IOSWindow*)window;
    NSString* text = [[NSString alloc] initWithUTF8String:keyboardData.text.c_str()];
    [win showKeyboard:text show:keyboardData.active];
}

