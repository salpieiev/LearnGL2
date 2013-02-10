//
//  OpenGLView.m
//  TeachGL2
//
//  Created by Sergey Alpeev on 12.01.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#import "OpenGLView.h"
#import <QuartzCore/QuartzCore.h>
#import "Renderers.h"



@interface OpenGLView () {
    RenderingEngine *renderer;
    
    GLuint resolveFramebuffer;
    GLuint sampleFramebuffer;
    GLuint colorRenderbuffer;
    GLuint sampleColorRenderbuffer;
    GLuint sampleDepthRenderbuffer;
}

- (void)drawView:(CADisplayLink *)displayLink;

@end



@implementation OpenGLView

#pragma mark - Class methods

+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

#pragma mark - Lifecycle

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        CGFloat scale = [UIScreen mainScreen].scale;
        CGFloat width = CGRectGetWidth(frame) * scale;
        CGFloat height = CGRectGetHeight(frame) * scale;
        
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.contentsScale = scale;
        
        self.eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        
        if (!self.eaglContext || ![EAGLContext setCurrentContext:self.eaglContext])
            return nil;
        
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [self.eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        
        glGenFramebuffers(1, &resolveFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, resolveFramebuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        glGenFramebuffers(1, &sampleFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, sampleFramebuffer);
        
        glGenRenderbuffers(1, &sampleColorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, sampleColorRenderbuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_RGBA8_OES, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, sampleColorRenderbuffer);
        
        glGenRenderbuffers(1, &sampleDepthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, sampleDepthRenderbuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24_OES, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, sampleDepthRenderbuffer);
        
        renderer = new Renderer4(width, height);
        
        CADisplayLink *displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(drawView:)];
        [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    }
    return self;
}

- (void)dealloc
{
    delete renderer;
}

#pragma mark - Private Methods

- (void)drawView:(CADisplayLink *)displayLink
{
    glBindFramebuffer(GL_FRAMEBUFFER, sampleFramebuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, sampleColorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, sampleDepthRenderbuffer);
    
    renderer->Render();
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, resolveFramebuffer);
    glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, sampleFramebuffer);
    glResolveMultisampleFramebufferAPPLE();
    
    const GLenum discards[] =
    {
        GL_COLOR_ATTACHMENT0,
        GL_DEPTH_ATTACHMENT
    };
    glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE, 2, discards);
    
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [self.eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

#pragma mark - Touches

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    CGFloat scale = [UIScreen mainScreen].scale;
    
    ivec2 location = ivec2(touchLocation.x * scale, touchLocation.y * scale);
    renderer->OnFingerDown(location);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint oldTouchLocation = [touch previousLocationInView:self];
    CGPoint newTouchLocation = [touch locationInView:self];
    CGFloat scale = [UIScreen mainScreen].scale;
    
    ivec2 oldLocation = ivec2(oldTouchLocation.x * scale, oldTouchLocation.y * scale);
    ivec2 newLocation = ivec2(newTouchLocation.x * scale, newTouchLocation.y * scale);
    renderer->OnFingerMove(oldLocation, newLocation);
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    CGFloat scale = [UIScreen mainScreen].scale;
    
    ivec2 location = ivec2(touchLocation.x * scale, touchLocation.y * scale);
    renderer->OnFingerUp(location);
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint touchLocation = [touch locationInView:self];
    CGFloat scale = [UIScreen mainScreen].scale;
    
    ivec2 location = ivec2(touchLocation.x * scale, touchLocation.y * scale);
    renderer->OnFingerUp(location);
}

@end
