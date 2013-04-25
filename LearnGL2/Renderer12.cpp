//
//  Renderer12.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 4/23/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer12.h"
#include "Shaders/NormalMapping.vsh"
#include "Shaders/NormalMapping.fsh"



struct Vertex
{
    vec3 Position;
    vec2 TexCoords;
};



Renderer12::Renderer12(int width, int height): RenderingEngine(width, height)
{
    m_rotator = new Rotator(m_surfaceSize);
    
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    PrepareProgram();
    GenerateBuffers();
    LoadTexture();
    
    float h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
}

Renderer12::~Renderer12()
{
    delete m_rotator;
}

void Renderer12::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DrawSphere();
}

void Renderer12::OnFingerDown(ivec2 location)
{
    m_rotator->Start(location);
}

void Renderer12::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    m_rotator->Move(newLocation);
}

void Renderer12::OnFingerUp(ivec2 location)
{
    m_rotator->End(location);
}

void Renderer12::PrepareProgram()
{
    m_program = BuildProgram(NormalMappingVertexShader, NormalMappingFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "a_position");
    m_attribTexCoord = glGetAttribLocation(m_program, "a_texCoord");
    
    m_uniformProjection = glGetUniformLocation(m_program, "u_projection");
    m_uniformModelview = glGetUniformLocation(m_program, "u_modelview");
}

void Renderer12::GenerateBuffers()
{
    Sphere sphere(2.0f, vec2(1.0f, 1.0f));
    
    vector<float> vertices;
    sphere.GenerateVertices(vertices, VertexFlagsTexCoords);
    
    vector<GLushort> indices;
    sphere.GenerateTriangleIndices(indices);
    m_indexCount = indices.size();
    
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
}

void Renderer12::LoadTexture()
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
//    SetPngPOTTexture("tile_floor.png");
    SetPVRTexture("NormalMap.pvr");
}

void Renderer12::DrawSphere() const
{
    mat4 rotation = m_rotator->GetOrientation().ToMatrix();
    
    mat4 modelview = mat4::Translate(0.0f, 0.0f, -7.0f);
    modelview = rotation * modelview;
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribTexCoord);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position)));
    
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribTexCoord);
}




