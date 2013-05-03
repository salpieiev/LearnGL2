//
//  Renderer14.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 4/30/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer14.h"
#include "Shaders/NormalMapGenerator.vsh"
#include "Shaders/NormalMapGenerator.fsh"
#include "ParametricSurface.h"



struct Vertex
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
    vec3 Tangent;
};



Renderer14::Renderer14(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    
    PrepareProgram();
    LoadTexture();
    SetupUniforms();
    GenerateBuffers();
}

Renderer14::~Renderer14()
{
    
}

void Renderer14::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DrawUnfoldedSurface();
}

void Renderer14::OnFingerDown(ivec2 location)
{
    
}

void Renderer14::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer14::OnFingerUp(ivec2 location)
{
    
}

void Renderer14::PrepareProgram()
{
    m_program = BuildProgram(NormalMapGeneratorVertexShader, NormalMapGeneratorFragmentShader);
    glUseProgram(m_program);
    
    m_attribNormal = glGetAttribLocation(m_program, "a_normal");
    m_attribTangent = glGetAttribLocation(m_program, "a_tangent");
    m_attribTexCoord = glGetAttribLocation(m_program, "a_texCoord");
    
    m_uniformProjection = glGetUniformLocation(m_program, "u_projection");
}

void Renderer14::GenerateBuffers()
{
//    Sphere sphere(1.8, vec2(1.0f, 1.0f));
    KleinBottle sphere(1.8, vec2(1.0f, 1.0f));
    
    vector<GLfloat> vertices;
    sphere.GenerateVertices(vertices, VertexFlagsNormals | VertexFlagsTangents | VertexFlagsTexCoords);
    
    vector<GLushort> indices;
    sphere.GenerateTriangleIndices(indices);
    m_indexCount = indices.size();
    
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

void Renderer14::LoadTexture()
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    SetPVRTexture("NormalMapEarth.pvr");
    
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Renderer14::SetupUniforms() const
{
    GLfloat h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
}

void Renderer14::DrawUnfoldedSurface() const
{
    glEnableVertexAttribArray(m_attribNormal);
    glEnableVertexAttribArray(m_attribTangent);
    glEnableVertexAttribArray(m_attribTexCoord);
    
    glVertexAttribPointer(m_attribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    glVertexAttribPointer(m_attribTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Normal)));
    glVertexAttribPointer(m_attribTangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Normal) + sizeof(Vertex::TexCoord)));
    
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribNormal);
    glDisableVertexAttribArray(m_attribTangent);
    glDisableVertexAttribArray(m_attribTexCoord);
}



