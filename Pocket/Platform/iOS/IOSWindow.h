//
//  Engine.h
//  Simple
//
//  Created by Jeppe Nielsen on 1/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include "Event.hpp"

@interface IOSWindow : UIWindow <UIApplicationDelegate, UITextViewDelegate> {
    CADisplayLink* gameLoopTimer;
    UITextView* keyboard;
}

-(void) showKeyboard:(NSString*)text show:(BOOL)shouldShow;
@end
