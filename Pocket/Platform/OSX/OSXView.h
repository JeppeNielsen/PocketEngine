//
//  Engine.h
//  Simple
//
//  Created by Jeppe Nielsen on 1/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface OSXView : NSOpenGLView <NSWindowDelegate>
{
    NSTimer *renderTimer;
    NSWindow* window;
}

- (void) setWindow:(NSWindow*) win;
- (void) drawRect: (NSRect) bounds;
- (void) prepareOpenGL;
- (void)windowDidResize:(NSNotification *)n;
-(NSMenuItem*) createMenuItem: (NSMenu*)menu withText:(NSString*)text withObject:(void*)object withShortCut:(NSString*)shortCut;
-(void) menuItemClicked:(id)sender;
-(NSPoint) convertViewLocationToWorldPoint: (NSPoint) point;
@end