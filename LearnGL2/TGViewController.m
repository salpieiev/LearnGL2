//
//  TGViewController.m
//  LearnGL2
//
//  Created by Sergey Alpeev on 12.01.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#import "TGViewController.h"
#import "OpenGLView.h"



@interface TGViewController ()

@end



@implementation TGViewController

#pragma mark - Lifecycle

- (id)init
{
    self = [super init];
    if (self) {
        
    }
    return self;
}

#pragma mark - View Lifecycle

- (void)loadView
{
    CGRect screenBounds = [UIScreen mainScreen].bounds;
    
    OpenGLView *openGLView = [[OpenGLView alloc] initWithFrame:screenBounds];
    self.view = openGLView;
}

@end
