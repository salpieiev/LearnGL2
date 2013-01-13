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
    
    GLuint resolveFramebuffer;
    GLuint sampleFramebuffer;
    GLuint colorRenderbuffer;
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
        CGFloat scale = [UIScreen mainScreen].scale;
        CGFloat width = CGRectGetWidth(frame) * scale;
        CGFloat height = CGRectGetHeight(frame) * scale;
        
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        eaglLayer.opaque = YES;
        eaglLayer.contentsScale = [UIScreen mainScreen].scale;
        
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
        
        GLuint sampleColorRenderbuffer;
        glGenRenderbuffers(1, &sampleColorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, sampleColorRenderbuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_RGBA8_OES, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, sampleColorRenderbuffer);
        
        GLuint sampleDepthRenderbuffer;
        glGenRenderbuffers(1, &sampleDepthRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, sampleDepthRenderbuffer);
        glRenderbufferStorageMultisampleAPPLE(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, width, height);
        
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

@end

/*//
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
 GLuint resolveFramebuffer;
 GLuint sampleFramebuffer;
 GLuint colorRenderbuffer;
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
 CGFloat scale = [UIScreen mainScreen].scale;
 CGFloat width = CGRectGetWidth(frame) * scale;
 CGFloat height = CGRectGetHeight(frame) * scale;
 
 CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
 eaglLayer.opaque = YES;
 eaglLayer.contentsScale = scale;
 
 self.eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
 [EAGLContext setCurrentContext:self.eaglContext];
 
 glGenFramebuffers(1, &resolveFramebuffer);
 glBindFramebuffer(GL_FRAMEBUFFER, resolveFramebuffer);
 
 glGenRenderbuffers(1, &colorRenderbuffer);
 glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
 
 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
 
 
 glGenFramebuffers(1, &sampleFramebuffer);
 glBindFramebuffer(GL_FRAMEBUFFER, sampleFramebuffer);
 
 GLuint sampleColorRenderbuffer;
 glGenRenderbuffers(1, &sampleColorRenderbuffer);
 glBindRenderbuffer(GL_RENDERBUFFER, sampleColorRenderbuffer);
 glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_RGBA8_OES, width, height);
 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, sampleColorRenderbuffer);
 
 GLuint sampleDepthRenderbuffer;
 glGenRenderbuffers(1, &sampleDepthRenderbuffer);
 glBindRenderbuffer(GL_RENDERBUFFER, sampleDepthRenderbuffer);
 glRenderbufferStorageMultisampleAPPLE(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT16, width, height);
 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, sampleDepthRenderbuffer);
 
 
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
 
 glBindFramebuffer(GL_FRAMEBUFFER, sampleFramebuffer);
 glViewport(0, 0, width, height);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
 renderer->Render(width, height);
 
 //    glBindFramebuffer(GL_FRAMEBUFFER, resolveFramebuffer);
 
 glBindFramebuffer(GL_DRAW_FRAMEBUFFER_APPLE, resolveFramebuffer);
 glBindFramebuffer(GL_READ_FRAMEBUFFER_APPLE, sampleFramebuffer);
 glResolveMultisampleFramebufferAPPLE();
 
 const GLenum discards[]  = {GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT};
 glDiscardFramebufferEXT(GL_READ_FRAMEBUFFER_APPLE,2,discards);
 
 glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
 [self.eaglContext presentRenderbuffer:GL_RENDERBUFFER];
 }
 
 @end
*/
