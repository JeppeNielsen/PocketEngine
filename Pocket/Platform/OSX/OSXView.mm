//
//  OSXWindow
//
//  Created by Jeppe Nielsen on 9/5/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#import "OSXView.h"
#include "OSXWindowCreator.h"
#include <OpenGL/gl.h>
#include "Window.hpp"
#include "AppMenu.hpp"

@implementation OSXView


-(void) drawRect: (NSRect) bounds
{
    Pocket::OSXWindowCreator::Instance()->OnUpdate(true);
    
}

- (void)prepareOpenGL {
    [super prepareOpenGL];
    
    
    NSTimeInterval timeInterval = 0.005;
    
    renderTimer = [ [ NSTimer scheduledTimerWithTimeInterval:timeInterval
                                                      target:self
                                                    selector:@selector( updateGLView: )
                                                    userInfo:nil repeats:YES ] retain ];
    [ [ NSRunLoop currentRunLoop ] addTimer:renderTimer
                                    forMode:NSEventTrackingRunLoopMode ];
    [ [ NSRunLoop currentRunLoop ] addTimer:renderTimer
                                    forMode:NSModalPanelRunLoopMode ];
    GLint defaultFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &defaultFBO);
    
    Pocket::Window::Framebuffer = (uint)defaultFBO;
    
    NSApplication* app = [NSApplication sharedApplication];
    NSMenu* menu = [[NSMenu alloc] initWithTitle: @""];

    {
        [menu addItemWithTitle:@"" action:nil keyEquivalent:@""];
    }

    [app setMainMenu:menu];
}

- (void) updateGLView:(NSTimer *)timer
{
    [self drawRect: [self frame]];
}

- (void) mouseDown:(NSEvent*)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    Pocket::OSXWindowCreator::Instance()->Down(0, location.x, location.y);
}

-(void) mouseUp:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    Pocket::OSXWindowCreator::Instance()->Up(0, location.x, location.y);
}

-(void) mouseDragged:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    Pocket::OSXWindowCreator::Instance()->Move(0, location.x, location.y);
}

-(void)mouseMoved:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    Pocket::OSXWindowCreator::Instance()->Set(0, location.x, location.y);
    Pocket::OSXWindowCreator::Instance()->Set(1, location.x, location.y);
    Pocket::OSXWindowCreator::Instance()->Set(2, location.x, location.y);
}

- (void)scrollWheel:(NSEvent *)theEvent {
    Pocket::OSXWindowCreator::Instance()->SetScroll(theEvent.scrollingDeltaY);
}

-(NSPoint) convertLocation: (NSPoint) point {
    NSPoint p;
    p.x = point.x;
    p.y = point.y;//_bounds.size.height - point.y;
    return [self convertPointToBacking:p];;
}

-(void)rightMouseDown:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    Pocket::OSXWindowCreator::Instance()->Down(1, location.x, location.y);
}

-(void)rightMouseUp:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    Pocket::OSXWindowCreator::Instance()->Up(1, location.x, location.y);
}

-(void)rightMouseDragged:(NSEvent *)theEvent {
    NSPoint location = [self convertLocation:theEvent.locationInWindow];
    Pocket::OSXWindowCreator::Instance()->Move(1, location.x, location.y);
}

-(void)otherMouseDown:(NSEvent *)theEvent {
    if (theEvent.type == NSOtherMouseDown) {
        NSPoint location = [self convertLocation:theEvent.locationInWindow];
        Pocket::OSXWindowCreator::Instance()->Down(2, location.x, location.y);
    }
}

-(void)otherMouseUp:(NSEvent *)theEvent {
    if (theEvent.type == NSOtherMouseUp) {
        NSPoint location = [self convertLocation:theEvent.locationInWindow];
        Pocket::OSXWindowCreator::Instance()->Up(2, location.x, location.y);
    }
}

-(void)otherMouseDragged:(NSEvent *)theEvent {
    if (theEvent.type == NSOtherMouseDragged) {
        NSPoint location = [self convertLocation:theEvent.locationInWindow];
        Pocket::OSXWindowCreator::Instance()->Move(2, location.x, location.y);
    }
}

-(void)keyDown:(NSEvent *)theEvent {
    unichar cd = [[theEvent characters] characterAtIndex:0];
    NSString* str = [NSString stringWithCharacters:&cd length:1];
    
    Pocket::OSXWindowCreator::Instance()->ButtonDown([str UTF8String]);
}

-(void)keyUp:(NSEvent *)theEvent {
    unichar cd = [[theEvent characters] characterAtIndex:0];
    NSString* str = [NSString stringWithCharacters:&cd length:1];
    
    Pocket::OSXWindowCreator::Instance()->ButtonUp([str UTF8String]);
}

- (void)windowDidResize:(NSNotification *)notification {
    NSLog(@"test");
}

std::map<NSMenuItem*, Pocket::AppMenu*> menuItemToAppMenu;

-(NSMenuItem*)createMenuItem:(NSMenu *)menu withText:(NSString*)text withObject:(void*)object withShortCut:(NSString*)shortCut
{
    NSMenuItem* menuItem = [menu addItemWithTitle:text action:@selector(menuItemClicked:) keyEquivalent:shortCut];
    menuItemToAppMenu[menuItem] = (Pocket::AppMenu*)object;
    return menuItem;
}

-(void)menuItemClicked:(id)sender
{
    NSMenuItem* menuItem = (NSMenuItem*)sender;
    menuItemToAppMenu[menuItem]->Clicked();
}

@end