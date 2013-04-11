//
//  OpenGLView.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 12.01.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#import <OpenGLES/EAGL.h>



@interface OpenGLView : UIView

@property (strong, nonatomic) EAGLContext *eaglContext;

@end
