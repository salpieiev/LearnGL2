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
#include "Shaders/PointSpriteShader.vsh"
#include "Shaders/PointSpriteShader.fsh"



Renderer::Renderer()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    resourceManager = new ResourceManager();
    
    programs.simpleProgram = BuildProgram(VertexShader, FragmentShader);
    programs.pointSpriteProgram = BuildProgram(PointSpriteVertexShader, PointSpriteFragmentShader);
    
    glUseProgram(programs.simpleProgram);
    attributes.Position = glGetAttribLocation(programs.simpleProgram, "Position");
    attributes.SourceColor = glGetAttribLocation(programs.simpleProgram, "SourceColor");
    glEnableVertexAttribArray(attributes.Position);
    
    glUseProgram(programs.pointSpriteProgram);
    attributes.PointSpritePosition = glGetAttribLocation(programs.pointSpriteProgram, "Position");
    uniforms.PointSpriteUniform = glGetUniformLocation(programs.pointSpriteProgram, "Sampler");
    glEnableVertexAttribArray(attributes.PointSpritePosition);
    
    GenTriangleVBO();
    GenPointSprite();
}

Renderer::~Renderer()
{
    delete resourceManager;
}

void Renderer::Render(int width, int height) const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, width, height);
    
    DrawTriangleWithoutVBO();
    
    DrawTriangleWithVBO();
    
    DrawPointSprites();
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
        -0.4f, -0.4f, 0.0f,
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

void Renderer::GenPointSprite()
{
    resourceManager->LoadPngImage("Star.png");
    
    ivec2 imageSize = resourceManager->GetImageSize();
    void *imageData = resourceManager->GetImageData();
    
    glGenTextures(1, &textures.Star);
    glBindTexture(GL_TEXTURE_2D, textures.Star);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageSize.x, imageSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    
    resourceManager->UnloadImage();
}

void Renderer::DrawPointSprites() const
{
    glUseProgram(programs.pointSpriteProgram);
    
    glBindTexture(GL_TEXTURE_2D, textures.Star);
    
    GLfloat spritePositions[] =
    {
        0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.0f
    };
    
    glVertexAttribPointer(attributes.PointSpritePosition, 3, GL_FLOAT, GL_FALSE, 0, spritePositions);
    
    glEnable(GL_BLEND);
    glDrawArrays(GL_POINTS, 0, 2);
    glDisable(GL_BLEND);
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
