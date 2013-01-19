//
//  Renderer.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 12.01.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer.h"



#define STRINGIFY(A)    #A
#include "Shaders/Shader.vsh"
#include "Shaders/Shader.fsh"



Renderer::Renderer()
{
    programs.simpleProgram = BuildProgram(VertexShader, FragmentShader);
    
    attributes.Position = glGetAttribLocation(programs.simpleProgram, "Position");
    attributes.SourceColor = glGetAttribLocation(programs.simpleProgram, "SourceColor");
    
    glEnableVertexAttribArray(attributes.Position);
    
    GenTriangleVBO();
}

Renderer::~Renderer()
{
    
}

void Renderer::Render(int width, int height) const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, width, height);
    
    DrawTriangleWithoutVBO();
    
    DrawTriangleWithVBO();
}

void Renderer::Update()
{
    
}

GLuint Renderer::BuildShader(const char *source, GLenum shaderType) const
{
    GLuint shader = glCreateShader(shaderType);
    
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    GLint compileSuccess;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
    
    if (compileSuccess == GL_FALSE)
    {
        GLint infoLenght;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLenght);
        
        if (infoLenght > 1)
        {
            char *infoLog = (char *)malloc(sizeof(char) * infoLenght);
            
            glGetShaderInfoLog(shader, infoLenght, NULL, infoLog);
            SALog("SHADER %d: %s\n", shaderType, infoLog);
            
            free(infoLog);
        }
        
        glDeleteShader(shader);
        
        return 0;
    }
    
    return shader;
}

GLuint Renderer::BuildProgram(const char *vertexShaderSource, const char *fragmentShaderSource) const
{
    GLuint vertexShader = BuildShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = BuildShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    
    glLinkProgram(program);
    
    GLint linkSuccess;
    glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess);
    
    if (linkSuccess == GL_FALSE)
    {
        GLint infoLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        
        if (infoLength > 1)
        {
            char *infoLog = (char *)malloc(sizeof(char) * infoLength);
            
            glGetProgramInfoLog(program, infoLength, NULL, infoLog);
            SALog("PROGRAM: %s\n", infoLog);
            
            free(infoLog);
        }
        
        glDeleteProgram(program);
        return 0;
    }
    
    return program;
}

void Renderer::GenTriangleVBO()
{
    GLfloat triangleData[] =
    {
        -1.0f, -1.0f, 0.0f,
        -0.25f, 0.25f, 0.0f,
        -0.75f, 0.0f, 0.0f
    };
    
    glGenBuffers(1, &vertexBuffers.triangleBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers.triangleBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData), NULL, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(triangleData), triangleData);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::DrawTriangleWithVBO() const
{
    glUseProgram(programs.simpleProgram);
    
    glVertexAttrib4f(attributes.SourceColor, 1, 0, 0, 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers.triangleBuffer);
    
    glVertexAttribPointer(attributes.Position, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::DrawTriangleWithoutVBO() const
{
    glUseProgram(programs.simpleProgram);
    
    glVertexAttrib4f(attributes.SourceColor, 0.5, 1, 0, 1);
    
    GLfloat vVertices[] =
    {
        0.0f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };
    
    glVertexAttribPointer(attributes.Position, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
}



void SALog(const char *formatStr, ...)
{
    va_list params;
    char buf[BUFSIZ];
    
    va_start(params, formatStr);
    vsprintf(buf, formatStr, params);
    
    printf("%s", buf);
    
    va_end(params);
}