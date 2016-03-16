//
//  AppDelegate.m
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/5/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#import "AppDelegate.h"
#import "OSXView.h"
#include "OSXWindowCreator.h"

@implementation AppDelegate

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    int windowWidth = Pocket::OSXWindowCreator::Instance()->Width;
    int windowHeight = Pocket::OSXWindowCreator::Instance()->Height;
    bool isFullScreen = Pocket::OSXWindowCreator::Instance()->FullScreen;
    
    NSRect mainScreenSize = [[NSScreen mainScreen] frame];
    
    CGFloat xPos = mainScreenSize.size.width/2 - windowWidth/2;
    CGFloat yPos = mainScreenSize.size.height/2 - windowHeight/2;
    
    if (isFullScreen) {
        //windowWidth = mainScreenSize.size.width + [[[NSScreen screens] objectAtIndex:1] frame].size.width;
        //windowHeight = mainScreenSize.size.height;
        xPos = 0;
        yPos = 0;
        //Pocket::OSXWindowCreator::Instance()->Width = windowWidth;
        //Pocket::OSXWindowCreator::Instance()->Height = windowHeight;
    }
    
    NSRect frame = NSMakeRect(xPos, yPos, windowWidth, windowHeight);
    NSUInteger styleMask = NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask | NSMiniaturizableWindowMask;
    if (isFullScreen) {
        styleMask |=NSFullScreenWindowMask;
    }
    NSRect rect = [NSWindow contentRectForFrameRect:frame styleMask:styleMask];
    window =  [[OSXWindow alloc] initWithContentRect:rect styleMask:styleMask backing: NSBackingStoreBuffered    defer:false];
    [window makeKeyAndOrderFront: window];
    [window setDelegate:self];
    [window setAcceptsMouseMovedEvents:YES];
    
    [ NSApp setDelegate:self ];
    renderTimer = nil;
    
    GLuint attributes[] = {
        NSOpenGLPFANoRecovery,
        NSOpenGLPFAWindow,
       // NSOpenGLPFAAccelerated,
        //NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 16,
        NSOpenGLPFAStencilSize, 8,
        NSOpenGLPFAAccumSize, 0,
        0
    };
    
    NSOpenGLPixelFormat* fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:(NSOpenGLPixelFormatAttribute*) attributes];
    if (!fmt)
        NSLog(@"No OpenGL pixel format");
    

    view = [[OSXView alloc]init];
    
    [window setContentView:view ];  
    
    [window makeFirstResponder:view];
    
    [view setPixelFormat:fmt];

    NSTrackingAreaOptions options = (NSTrackingActiveAlways | NSTrackingInVisibleRect |  
                         NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved);

    NSTrackingArea *area = [[NSTrackingArea alloc] initWithRect:[view bounds]
                                                    options:options
                                                      owner:window
                                                   userInfo:nil];

    
    [view addTrackingArea:area];

    
    Pocket::OSXWindowCreator::Instance()->OnInitialize(view);
    
    //[view enterFullScreenMode:[NSScreen mainScreen] withOptions:nil];
}

- (void)windowWillClose:(NSNotification *)notification
{
    exit(0);
}

- (void)windowDidResize:(NSNotification *)notification {
    Pocket::OSXWindowCreator::Instance()->ScreenSizeChanged(view.bounds.size.width, view.bounds.size.height);
}

@end
