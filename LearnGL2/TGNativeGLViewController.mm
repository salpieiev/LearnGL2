//
//  TGViewController.m
//  GLSource
//
//  Created by Sergey Alpeev on 02.02.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#import "TGNativeGLViewController.h"
#import "Renderers.h"



@interface TGNativeGLViewController () {
    RenderingEngine *renderer;
}

@property (strong, nonatomic) EAGLContext *context;

@end



@implementation TGNativeGLViewController

- (id)init
{
    self = [super init];
    if (self) {
        self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        [EAGLContext setCurrentContext:self.context];
        
        CGSize screenSize = [UIScreen mainScreen].bounds.size;
        CGFloat scale = [UIScreen mainScreen].scale;
        
        CGFloat width = screenSize.width * scale;
        CGFloat height = screenSize.height * scale;
        
        renderer = new Renderer9(width, height);
    }
    return self;
}

#pragma mark - View lifecykle

- (void)loadView
{
    CGSize screenSize = [UIScreen mainScreen].bounds.size;
    CGRect glkViewFrame = CGRectMake(0.0f, 0.0f, screenSize.width, screenSize.height);
    
    GLKView *glkView = [[GLKView alloc] initWithFrame:glkViewFrame
                                              context:self.context];
    glkView.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    glkView.drawableMultisample = GLKViewDrawableMultisample4X;
    glkView.drawableStencilFormat = GLKViewDrawableStencilFormat8;
    self.view = glkView;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{
    
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    renderer->Render();
}

#pragma mark - Touches

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint touchLocation = [touch locationInView:self.view];
    CGFloat scale = [UIScreen mainScreen].scale;
    
    ivec2 location = ivec2(touchLocation.x * scale, touchLocation.y * scale);
    renderer->OnFingerDown(location);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint oldTouchLocation = [touch previousLocationInView:self.view];
    CGPoint newTouchLocation = [touch locationInView:self.view];
    CGFloat scale = [UIScreen mainScreen].scale;
    
    ivec2 oldLocation = ivec2(oldTouchLocation.x * scale, oldTouchLocation.y * scale);
    ivec2 newLocation = ivec2(newTouchLocation.x * scale, newTouchLocation.y * scale);
    renderer->OnFingerMove(oldLocation, newLocation);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint touchLocation = [touch locationInView:self.view];
    CGFloat scale = [UIScreen mainScreen].scale;
    
    ivec2 location = ivec2(touchLocation.x * scale, touchLocation.y * scale);
    renderer->OnFingerUp(location);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint touchLocation = [touch locationInView:self.view];
    CGFloat scale = [UIScreen mainScreen].scale;
    
    ivec2 location = ivec2(touchLocation.x * scale, touchLocation.y * scale);
    renderer->OnFingerUp(location);
}

@end
