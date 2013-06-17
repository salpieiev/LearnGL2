//
//  Renderer17.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 17.06.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer17.h"
#include "ParametricSurface.h"
#include "Shaders/GaussianBloomTextureShader.vsh"
#include "Shaders/GaussianBloomTextureShader.fsh"
#include "Shaders/GaussianBloomSurfaceShader.vsh"
#include "Shaders/GaussianBloomSurfaceShader.fsh"



struct Vertex
{
    vec3 Position;
    vec4 Color;
    vec3 Normal;
};



Renderer17::Renderer17(int width, int height): RenderingEngine(width, height)
{
    BuildTextureProgram();
    LoadTexture();
    
    BuildSurfaceProgram();
    GenerateSurfaceBuffers();
    
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    /*// Create the depth buffer for the full-size off-screen FBO
    glGenRenderbuffers(1, &m_renderbuffers.SceneDepthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers.SceneDepthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    
    glGenTextures(1, &m_textures.SceneTexture);
    glBindTexture(GL_TEXTURE_2D, m_textures.SceneTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    
    glGenFramebuffers(1, &m_framebuffers.SceneFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.SceneFramebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffers.SceneDepthRenderbuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures.SceneTexture, 0);*/
    
    /*// Create FBOs for the half, quarter and eight sizes
    int w = width;
    int h = height;
    
    for (int i = 0; i < OffscreenCountGaussian; i++, w >>= 1, h >>= 1)
    {
        glGenFramebuffers(1, &m_framebuffers.OffscreenFramebuffers[i]);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.OffscreenFramebuffers[i]);
        
        glGenTextures(1, &m_textures.OffscreenTextures[i]);
        glBindTexture(GL_TEXTURE_2D, m_textures.OffscreenTextures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures.OffscreenTextures[i], 0);
    }
    
    GenerateBloomTexture();*/
}

Renderer17::~Renderer17()
{
    
}

void Renderer17::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0, 0, m_surfaceSize.x, m_surfaceSize.y);
    glBindTexture(GL_TEXTURE_2D, m_textures.BackgroundTexture);
    DrawTexture();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    DrawSurface();
}

void Renderer17::OnFingerDown(ivec2 location)
{
    
}

void Renderer17::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer17::OnFingerUp(ivec2 location)
{
    
}

void Renderer17::BuildTextureProgram()
{
    m_textureProgram = BuildProgram(GaussianBloomTextureVertexShader, GaussianBloomTextureFragmentShader);
    glUseProgram(m_textureProgram);
    
    m_attribTexturePosition = glGetAttribLocation(m_textureProgram, "a_position");
    m_attribTextureCoord = glGetAttribLocation(m_textureProgram, "a_texCoord");
    
    m_uniformTextureThreshold = glGetUniformLocation(m_textureProgram, "u_threshold");
}

void Renderer17::BuildSurfaceProgram()
{
    m_surfaceProgram = BuildProgram(GaussianBloomSurfaceVertexShader, GaussianBloomSurfaceFragmentShader);
    glUseProgram(m_surfaceProgram);
    
    m_attribSurfacePosition = glGetAttribLocation(m_surfaceProgram, "Position");
    m_attribSurfaceSourceColor = glGetAttribLocation(m_surfaceProgram, "SourceColor");
    m_attribSurfaceNormal = glGetAttribLocation(m_surfaceProgram, "Normal");
    m_uniformSurfaceProjection = glGetUniformLocation(m_surfaceProgram, "Projection");
    m_uniformSurfaceModelview = glGetUniformLocation(m_surfaceProgram, "Modelview");
    m_uniformSurfaceNormalMatrix = glGetUniformLocation(m_surfaceProgram, "NormalMatrix");
    m_uniformSurfaceLightPosition = glGetUniformLocation(m_surfaceProgram, "LightPosition");
    m_uniformSurfaceAmbientLight = glGetUniformLocation(m_surfaceProgram, "AmbientLight");
    m_uniformSurfaceSpecularLight = glGetUniformLocation(m_surfaceProgram, "SpecularLight");
    m_uniformSurfaceShininess = glGetUniformLocation(m_surfaceProgram, "Shininess");
}


void Renderer17::LoadTexture()
{
    glGenTextures(1, &m_textures.BackgroundTexture);
    glBindTexture(GL_TEXTURE_2D, m_textures.BackgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SetPngPOTTexture("bloom_background.png");
}

void Renderer17::GenerateSurfaceBuffers()
{
    // Create surface
    //    surface = Cone(5.0f, 1.8f);
    //    Cylinder surface(3.0f, 0.5f);
    //    surface = Sphere(2.0f);
    //    surface = Torus(1.8f, 0.5f);
    TrefoilKnot surface = TrefoilKnot(3.0f);
    //    surface = MobiusStrip(1.5f);
    //    surface = KleinBottle(0.3f);
    
    vector<float> vertices;
    surface.GenerateVertices(vertices, VertexFlagsColors | VertexFlagsNormals);
    
    vector<unsigned short> indices;
    surface.GenerateTriangleIndices(indices);
    m_surfaceIndexCount = indices.size();
    
    // Generate vertex buffer
    glGenBuffers(1, &m_surfaceVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    // Generate index buffer
    glGenBuffers(1, &m_surfaceIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_surfaceIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), &indices[0], GL_STATIC_DRAW);
    
    // Setup uniforms
    glUniform3f(m_uniformSurfaceLightPosition, 0.25f, 0.25f, 1.0f);
    glUniform3f(m_uniformSurfaceAmbientLight, 0.04f, 0.04f, 0.04f);
    glUniform3f(m_uniformSurfaceSpecularLight, 0.5f, 0.5f, 0.5f);
    glUniform1f(m_uniformSurfaceShininess, 50);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer17::DrawTexture() const
{
    GLfloat positions[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f
    };
    
    GLfloat texCoords[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };
    
    glUseProgram(m_textureProgram);
    
    glEnableVertexAttribArray(m_attribTexturePosition);
    glEnableVertexAttribArray(m_attribTextureCoord);
    
    glVertexAttribPointer(m_attribTexturePosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, positions);
    glVertexAttribPointer(m_attribTextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, texCoords);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(m_attribTexturePosition);
    glDisableVertexAttribArray(m_attribTextureCoord);
}

void Renderer17::DrawSurface() const
{
    glUseProgram(m_surfaceProgram);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_surfaceIndexBuffer);
    
    mat4 orientation = mat4();
    mat4 translation = mat4::Translate(0.0f, 0.0f, -7.0f);
    mat4 modelview = orientation * translation;
    mat3 normalMatrix = modelview.ToMat3();
    
    GLfloat h = 4 * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformSurfaceProjection, 1, GL_FALSE, projection.Pointer());
    
    glUniformMatrix4fv(m_uniformSurfaceModelview, 1, GL_FALSE, modelview.Pointer());
    glUniformMatrix3fv(m_uniformSurfaceNormalMatrix, 1, GL_FALSE, normalMatrix.Pointer());
    
    glEnableVertexAttribArray(m_attribSurfacePosition);
    glEnableVertexAttribArray(m_attribSurfaceSourceColor);
    glEnableVertexAttribArray(m_attribSurfaceNormal);
    
    glVertexAttribPointer(m_attribSurfacePosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribSurfaceSourceColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    glVertexAttribPointer(m_attribSurfaceNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Color)));
    
    glDrawElements(GL_TRIANGLES, m_surfaceIndexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribSurfacePosition);
    glDisableVertexAttribArray(m_attribSurfaceSourceColor);
    glDisableVertexAttribArray(m_attribSurfaceNormal);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer17::GenerateBloomTexture() const
{
    glViewport(0, 0, m_surfaceSize.x, m_surfaceSize.y);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.SceneFramebuffer);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(m_textureProgram);
    glUniform1f(m_uniformTextureThreshold, 0.0f);
    glBindTexture(GL_TEXTURE_2D, m_textures.BackgroundTexture);
    
    DrawTexture();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    DrawSurface();
    
    glUseProgram(m_textureProgram);
    glUniform1f(m_uniformTextureThreshold, 0.85f);
    
    // Downsample the rendered scene
    int w = m_surfaceSize.x;
    int h = m_surfaceSize.y;
    for (int i = 0; i < OffscreenCountGaussian; i++, w >>= 1, h >>= 1)
    {
        glViewport(0, 0, w, h);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.OffscreenFramebuffers[i]);
        glBindTexture(GL_TEXTURE_2D, i ? m_textures.OffscreenTextures[i - 1] : m_textures.SceneTexture);
        
        DrawTexture();
        
        glUseProgram(m_textureProgram);
        glUniform1f(m_uniformTextureThreshold, 0.0f);
    }
    
    // Create bloom texture
    glUseProgram(m_textureProgram);
    glUniform1f(m_uniformTextureThreshold, 0.0f);
    
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    
    glViewport(0, 0, m_surfaceSize.x, m_surfaceSize.y);
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers.OffscreenFramebuffers[0]);
    
    for (int i = 0; i < OffscreenCountGaussian; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_textures.OffscreenTextures[i]);
        DrawTexture();
    }
    
    glDisable(GL_BLEND);
}






