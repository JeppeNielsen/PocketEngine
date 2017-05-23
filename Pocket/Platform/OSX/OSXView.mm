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

-(void) setWindow:(NSWindow*) win {
    window = win;
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
    return point;
}

-(NSPoint) convertViewLocationToWorldPoint: (NSPoint) point {
    NSPoint p = [self convertPoint:point toView:nil];
    NSRect rect;
    rect.origin.x = p.x;
    rect.origin.y = p.y;
    
    rect = [window convertRectToScreen:rect];
    return rect.origin;
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

    Pocket::ModifierKey modifierKey = Pocket::ModifierKey::None;

    NSUInteger flags = [[NSApp currentEvent] modifierFlags];
    if ((flags & NSShiftKeyMask)) {
        modifierKey = Pocket::ModifierKey::Shift;
    } else if ((flags & NSCommandKeyMask)) {
        modifierKey = Pocket::ModifierKey::Command;
    } else if ((flags & NSAlternateKeyMask)) {
        modifierKey = Pocket::ModifierKey::Alt;
    } else if ((flags & NSControlKeyMask)) {
        modifierKey = Pocket::ModifierKey::Ctrl;
    }

    unichar cd = [[theEvent characters] characterAtIndex:0];
    NSString* str = [NSString stringWithCharacters:&cd length:1];
    Pocket::OSXWindowCreator::Instance()->ButtonDown([str UTF8String], modifierKey);
}

-(void)keyUp:(NSEvent *)theEvent {
    unichar cd = [[theEvent characters] characterAtIndex:0];
    NSString* str = [NSString stringWithCharacters:&cd length:1];
    
    Pocket::OSXWindowCreator::Instance()->ButtonUp([str UTF8String]);
}

std::map<NSMenuItem*, Pocket::AppMenu*> menuItemToAppMenu;

-(NSMenuItem*)createMenuItem:(NSMenu *)menu withText:(NSString*)text withObject:(void*)object withShortCut:(NSString*)shortCut
{
    NSMenuItem* menuItem = [[menu addItemWithTitle:text action:@selector(menuItemClicked:) keyEquivalent:shortCut] retain];
    menuItemToAppMenu[menuItem] = (Pocket::AppMenu*)object;
    return menuItem;
}

-(void)removeMenuItem:(NSMenu *)menu withItem:(void*)item {
    for(auto it : menuItemToAppMenu) {
        if (it.second == (Pocket::AppMenu*)item){
            [menu removeItem:it.first];
            menuItemToAppMenu.erase(menuItemToAppMenu.find(it.first));
            break;
        }
    }
}

-(void)menuItemClicked:(id)sender
{
    NSMenuItem* menuItem = (NSMenuItem*)sender;
    menuItemToAppMenu[menuItem]->Clicked();
}

@end
