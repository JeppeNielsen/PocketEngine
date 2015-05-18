//
//  IOSViewController.m
//  Pocket
//
//  Created by Jeppe Nielsen on 12/29/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#import "IOSViewController.h"

@implementation IOSViewController
-(BOOL) shouldAutorotate {
    return NO;
}
-(NSUInteger)supportedInterfaceOrientations {
    return isLandscape ? UIInterfaceOrientationMaskLandscape : UIInterfaceOrientationMaskPortrait;
}

@end
