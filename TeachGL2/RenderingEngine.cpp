//
//  RenderingEngine.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "RenderingEngine.h"



RenderingEngine::RenderingEngine(int width, int height): m_surfaceSize(width, height)
{
    m_resourceManager = new ResourceManager();
}

RenderingEngine::~RenderingEngine()
{
    delete m_resourceManager;
}

GLuint RenderingEngine::BuildShader(const char *source, GLenum shaderType) const
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

GLuint RenderingEngine::BuildProgram(const char *vertexShaderSource, const char *fragmentShaderSource) const
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



void SALog(const char *formatStr, ...)
{
    va_list params;
    char buf[BUFSIZ];
    
    va_start(params, formatStr);
    vsprintf(buf, formatStr, params);
    
    printf("%s", buf);
    
    va_end(params);
}

