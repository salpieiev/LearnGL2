//
//  TGAppDelegate.m
//  TeachGL2
//
//  Created by Sergey Alpeev on 12.01.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#import "TGAppDelegate.h"
#import "TGViewController.h"



@implementation TGAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.window.rootViewController = [[TGViewController alloc] init];
    [self.window makeKeyAndVisible];
    return YES;
}

@end
