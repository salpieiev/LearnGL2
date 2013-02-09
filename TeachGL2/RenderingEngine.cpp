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

void RenderingEngine::SetPngTexture(const string &name) const
{
    TextureDescription description = m_resourceManager->LoadPngImage(name);
    
    GLenum format = 0;
    switch (description.GetTexFormat())
    {
        case TextureFormatGray: format = GL_LUMINANCE; break;
        case TextureFormatGrayAlpha: format = GL_LUMINANCE_ALPHA; break;
        case TextureFormatRGB: format = GL_RGB; break;
        case TextureFormatRGBA: format = GL_RGBA; break;
        default: break;
    }
    
    GLenum type = 0;
    switch (description.GetBitsPerComponent())
    {
        // Check order. May be incorrect. Page 190 iPhone 3D Programming
        case 4:
        {
            if (format == GL_RGBA)
            {
                type = GL_UNSIGNED_SHORT_4_4_4_4;
                break;
            }
        }
        case 8: type = GL_UNSIGNED_BYTE; break;
        default: assert("Unsupported format");
    }
    
    void *data = description.GetTexData();
    ivec2 size = description.GetTexSize();
    
    glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, type, data);
}

void RenderingEngine::SetPVRTexture(const string &name) const
{
    TextureDescription description = m_resourceManager->LoadPVRImage(name);
    
    unsigned char *data = (unsigned char *)description.GetTexData();
    int bitsPerPixel = 0;
    GLenum format = 0;
    switch (description.GetTexFormat())
    {
        case TextureFormatPVRTC_RGBA2:
        {
            bitsPerPixel = 2;
            format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
            break;
        }
        case TextureFormatPVRTC_RGB2:
        {
            bitsPerPixel = 2;
            format = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
            break;
        }
        case TextureFormatPVRTC_RGBA4:
        {
            bitsPerPixel = 4;
            format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
            break;
        }
        case TextureFormatPVRTC_RGB4:
        {
            bitsPerPixel = 4;
            format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
            break;
        }
        default:
            break;
    }
    
    int width = description.GetTexSize().x;
    int height = description.GetTexSize().y;
    
    for (int level = 0; width > 0 && height > 0; level++)
    {
        GLsizei size = std::max(32, width * height * bitsPerPixel / 8);
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, data);
        data += size;
        width >>= 1;
        height >>= 1;
    }
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

