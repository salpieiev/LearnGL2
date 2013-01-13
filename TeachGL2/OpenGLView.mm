//
//  OpenGLView.m
//  TeachGL2
//
//  Created by Sergey Alpeev on 12.01.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#import "OpenGLView.h"
#import <QuartzCore/QuartzCore.h>
#import "Renderer.h"



@interface OpenGLView () {
    Renderer *renderer;
}

- (void)drawView;

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
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.contentsScale = [UIScreen mainScreen].scale;
        
        self.eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        [EAGLContext setCurrentContext:self.eaglContext];
        
        GLuint framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        
        GLuint renderbuffer;
        glGenRenderbuffers(1, &renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
        
        [self.eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:eaglLayer];
        
        renderer = new Renderer();
        
        [self drawView];
    }
    return self;
}

- (void)dealloc
{
    delete renderer;
}

#pragma mark - Private Methods

- (void)drawView
{
    CGFloat scale = [UIScreen mainScreen].scale;
    
    CGFloat width = CGRectGetWidth(self.bounds) * scale;
    CGFloat height = CGRectGetHeight(self.bounds) * scale;
    
    renderer->Render(width, height);
    
    [self.eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

@end
