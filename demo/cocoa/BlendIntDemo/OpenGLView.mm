//
//  OpenGLView.m
//  BlendIntDemo
//
//  Created by Freeman Zhang on 14-1-6.
//  Copyright (c) 2014年 Freeman Zhang. All rights reserved.
//

#import "OpenGLView.h"

#include <OpenGL/gl.h>

static void drawAnObject()
{
    glColor3f(1.0f, 0.85f, 0.35f);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(0.0, 0.6, 0.0);
        glVertex3f(-0.2, -0.3, 0.0);
        glVertex3f(0.2, -0.3, 0.0);
    }
    glEnd();
}

@implementation OpenGLView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (void)drawRect:(NSRect)bounds
{
	[super drawRect:bounds];
	
    // Drawing code here.
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    drawAnObject();
    glFlush();
}

@end
