//
//  TGAppDelegate.m
//  LearnGL2
//
//  Created by Sergey Alpeev on 12.01.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#import "TGAppDelegate.h"
#import "TGViewController.h"
#import "TGNativeGLViewController.h"



#define kUseNativeGLView            NO



@implementation TGAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.rootViewController = kUseNativeGLView ? [[TGNativeGLViewController alloc] init] : [[TGViewController alloc] init];
    [self.window makeKeyAndVisible];
    return YES;
}

@end
