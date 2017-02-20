
//
//  Engine.m
//  Simple
//
//  Created by Jeppe Nielsen on 1/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "IOSWindow.h"
#include "IOSWindowCreator.h"
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGLDrawable.h>
#import "IOSViewController.h"

@implementation IOSWindow

+ (Class) layerClass
{
	return [CAEAGLLayer class];
}

#define DegreesToRadians(degrees) (degrees * M_PI / 180)

-(id) init {
    
    [self setRootViewController:[[IOSViewController alloc]init]];
    
    
    
    bool isLandscape = Pocket::IOSWindowCreator::Instance()->isLandscape;
    
    CGRect screenBounds = [[UIScreen mainScreen] bounds];
    CGRect windowBounds = screenBounds;
    if (isLandscape) {
        windowBounds.size = CGSizeMake(screenBounds.size.height, screenBounds.size.width);
    }
    self = [super initWithFrame:windowBounds];
    if (self) {
        [[UIApplication sharedApplication] setStatusBarHidden:YES];
        if (isLandscape) {
            [self setTransform:
             CGAffineTransformConcat(
                                     CGAffineTransformMakeRotation(DegreesToRadians(90)),
                                     CGAffineTransformMakeTranslation(screenBounds.size.width * 0.5f - windowBounds.size.width * 0.5f, screenBounds.size.height * 0.5f - windowBounds.size.height * 0.5f))
             ];
        }
        keyboard = NULL;
        [self createKeyBoard];
    }
    return self;
}

-(void)showKeyboard:(NSString *)text show:(BOOL)shouldShow {
    
    if (shouldShow) {
        [self createKeyBoard];
        Pocket::IOSWindowCreator::Instance()->inputDevice->KeyboardText = std::string([text UTF8String]);
        keyboard.text = text;
        [keyboard becomeFirstResponder];
    } else {
        if (keyboard) {
            [keyboard resignFirstResponder];
            //[keyboard release];
            //keyboard = NULL;
        }
    }
    
    Pocket::IOSWindowCreator::Instance()->inputDevice->KeyboardActive = shouldShow;
}

-(void)createKeyBoard {
    if (!keyboard) {
        keyboard = [[UITextView alloc] initWithFrame:CGRectMake(-100, -100, 1, 1)];
        keyboard.delegate = self;
        [keyboard setReturnKeyType: UIReturnKeyDone];
        [self addSubview:keyboard];
        keyboard.autocorrectionType = UITextAutocorrectionTypeNo;
    }

}

- (void)textViewDidChange:(UITextView *)textView
{
    Pocket::IOSWindowCreator::Instance()->inputDevice->KeyboardText = std::string([textView.text UTF8String]);
}

- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text {
    
    if([text isEqualToString:@"\n"]) {
        [self showKeyboard:@"" show:false];
        return NO;
    }
    
    return YES;
}

- (void)textViewDidEndEditing:(UITextView *)textView {
    [self showKeyboard:@"" show:false];
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [self makeKeyAndVisible];
    self.backgroundColor = [UIColor blackColor];
    self.userInteractionEnabled = true;
    [self setMultipleTouchEnabled: YES];
    
    CAEAGLLayer *eaglLayer = (CAEAGLLayer*) self.layer;
    
    eaglLayer.opaque = YES;
    eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking, kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];

    gameLoopTimer = [CADisplayLink displayLinkWithTarget:self selector:@selector(gameLoop)];
    [gameLoopTimer setFrameInterval:1];
    [gameLoopTimer addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];

    Pocket::IOSWindowCreator::Instance()->OnInitialize((void*)self);
    
    return YES;
}

-(void) gameLoop {
    Pocket::IOSWindowCreator::Instance()->OnUpdate(true);
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    
    
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
     */
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    /*
     Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
     */
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    /*
     Called when the application is about to terminate.
     Save data if appropriate.
     See also applicationDidEnterBackground:.
     */
}

-(CGPoint) convertLocation: (CGPoint) point {
    CGPoint p;
    p.x = point.x;
    p.y = self.bounds.size.height - point.y;
    return p;
}

-(void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches)
    {
        CGPoint location = [self convertLocation:[touch locationInView:touch.view]];
        Pocket::IOSWindowCreator::Instance()->Down(touch.hash, location.x, location.y);
    }
}

-(void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches)
    {
        CGPoint location = [self convertLocation:[touch locationInView:touch.view]];
        Pocket::IOSWindowCreator::Instance()->Move(touch.hash, location.x, location.y);
    }
}

-(void) touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches)
    {
        CGPoint location = [self convertLocation:[touch locationInView:touch.view]];
        Pocket::IOSWindowCreator::Instance()->Up(touch.hash, location.x, location.y);
    }
}

-(void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
    for (UITouch *touch in touches)
    {
        CGPoint location = [self convertLocation:[touch locationInView:touch.view]];
        Pocket::IOSWindowCreator::Instance()->Up(touch.hash, location.x, location.y);
    }
}

@end
