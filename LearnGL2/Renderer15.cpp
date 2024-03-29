//
//  Renderer15.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 20.05.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer15.h"
#include "ParametricSurface.h"
#include "Shaders/BlurShader.vsh"
#include "Shaders/BlurShader.fsh"
#include "Shaders/BlurTextureShader.vsh"
#include "Shaders/BlurTextureShader.fsh"



struct Vertex
{
    vec3 Position;
    vec4 Color;
    vec3 Normal;
};



Renderer15::Renderer15(int width, int height): RenderingEngine(width, height), m_texture(-1)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    m_rotator = new Rotator(m_surfaceSize);
    
    BuildTextureProgram();
    BuildSurfaceProgram();
    
    GenerateSurfaceBuffers();
    
    GenerateTexture();
    
    glViewport(0, 0, width, height);
    
    // Set frustum
    GLfloat h = 4 * height / width;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
}

Renderer15::~Renderer15()
{
    delete m_rotator;
}

void Renderer15::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DrawBlurredTexture();
}

void Renderer15::OnFingerDown(ivec2 location)
{
    m_rotator->Start(location);
}

void Renderer15::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    m_rotator->Move(newLocation);
}

void Renderer15::OnFingerUp(ivec2 location)
{
    m_rotator->End(location);
}

void Renderer15::BuildSurfaceProgram()
{
    m_program = BuildProgram(BlurVertexShader, BlurFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "Position");
    m_attribSourceColor = glGetAttribLocation(m_program, "SourceColor");
    m_attribNormal = glGetAttribLocation(m_program, "Normal");
    m_uniformProjection = glGetUniformLocation(m_program, "Projection");
    m_uniformModelview = glGetUniformLocation(m_program, "Modelview");
    m_uniformNormalMatrix = glGetUniformLocation(m_program, "NormalMatrix");
    m_uniformLightPosition = glGetUniformLocation(m_program, "LightPosition");
    m_uniformAmbientLight = glGetUniformLocation(m_program, "AmbientLight");
    m_uniformSpecularLight = glGetUniformLocation(m_program, "SpecularLight");
    m_uniformShininess = glGetUniformLocation(m_program, "Shininess");
}

void Renderer15::BuildTextureProgram()
{
    m_textureProgram = BuildProgram(BlurTextureVertexShader, BlurTextureFragmentShader);
    glUseProgram(m_textureProgram);
    
    m_attribTexPosition = glGetAttribLocation(m_textureProgram, "a_position");
    m_attribTexCoord = glGetAttribLocation(m_textureProgram, "a_texCoord");
    m_uniformTexProjection = glGetUniformLocation(m_textureProgram, "u_projection");
    m_uniformTexModelview = glGetUniformLocation(m_textureProgram, "u_modelview");
    m_uniformTexBlurStep = glGetUniformLocation(m_textureProgram, "u_blurStep");
}

void Renderer15::GenerateSurfaceBuffers()
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
    m_indexCount = indices.size();
    
    // Generate vertex buffer
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    // Generate index buffer
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), &indices[0], GL_STATIC_DRAW);
    
    // Setup uniforms
    glUniform3f(m_uniformLightPosition, 0.25f, 0.25f, 1.0f);
    glUniform3f(m_uniformAmbientLight, 0.04f, 0.04f, 0.04f);
    glUniform3f(m_uniformSpecularLight, 0.5f, 0.5f, 0.5f);
    glUniform1f(m_uniformShininess, 50);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer15::DrawSurface() const
{
    glUseProgram(m_program);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    
    mat4 orientation = m_rotator->GetOrientation().ToMatrix();
    mat4 translation = mat4::Translate(0.0f, 0.0f, -7.0f);
    mat4 modelview = orientation * translation;
    mat3 normalMatrix = modelview.ToMat3();
    
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    glUniformMatrix3fv(m_uniformNormalMatrix, 1, GL_FALSE, normalMatrix.Pointer());
    
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribSourceColor);
    glEnableVertexAttribArray(m_attribNormal);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribSourceColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    glVertexAttribPointer(m_attribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Color)));
    
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribSourceColor);
    glDisableVertexAttribArray(m_attribNormal);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer15::DrawBlurredTexture() const
{
    glUseProgram(m_textureProgram);
    
    GLfloat vertices[] =
    {
        -2.0f, -2.0f, -5.0f,
        2.0f, -2.0f, -5.0f,
        2.0f, 2.0f, -5.0f,
        -2.0f, -2.0f, -5.0f,
        2.0f, 2.0f, -5.0f,
        -2.0f, 2.0f, -5.0f
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
    
    // Set frustum
    GLfloat h = 4 * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformTexProjection, 1, GL_FALSE, projection.Pointer());
    
    mat4 modelview;
    glUniformMatrix4fv(m_uniformTexModelview, 1, GL_FALSE, modelview.Pointer());
    
    glUniform1f(m_uniformTexBlurStep, 0.005f);
    
    glEnableVertexAttribArray(m_attribTexPosition);
    glEnableVertexAttribArray(m_attribTexCoord);
    
    glVertexAttribPointer(m_attribTexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, vertices);
    glVertexAttribPointer(m_attribTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, texCoords);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(m_attribTexPosition);
    glDisableVertexAttribArray(m_attribTexCoord);
}

void Renderer15::GenerateTexture()
{
    glUseProgram(m_program);
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2048, 2048, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLuint depthRenderbuffer;
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 2048, 2048);
    
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -2.0f, 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
    
    glViewport(0, 0, 2048, 2048);
    
    glClearColor(0.5f, 0.4f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DrawSurface();
}



