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
}

- (void) drawRect: (NSRect) bounds;
- (void) prepareOpenGL;
- (void)windowDidResize:(NSNotification *)n;

@end