//
//  AppDelegate.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/5/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "OSXView.h"

@interface AppDelegate : NSResponder <NSApplicationDelegate, NSWindowDelegate>
{
    NSTimer* renderTimer;
    NSWindow* window;
    OSXView* view;
}

@end
