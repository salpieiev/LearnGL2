//
//  Renderer2.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 2/4/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer2.h"
#include "Shaders/LightShader.vsh"
#include "Shaders/LightShader.fsh"



struct Vertex
{
    vec3 Position;
    vec4 Color;
    vec3 Normal;
    vec3 LightDirection;
};




Renderer2::Renderer2(int width, int height): RenderingEngine(width, height)
{
    m_rotator = new Rotator(m_surfaceSize);
    
    // Create and link program
    m_program = BuildProgram(LightVertexShader, LightFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "Position");
    m_attribSourceColor = glGetAttribLocation(m_program, "SourceColor");
    m_attribNormal = glGetAttribLocation(m_program, "Normal");
    m_attribLightDirection = glGetAttribLocation(m_program, "LightDirection");
    
    m_uniformProjection = glGetUniformLocation(m_program, "Projection");
    m_uniformModelview = glGetUniformLocation(m_program, "Modelview");
    m_uniformNormalMatrix = glGetUniformLocation(m_program, "NormalMatrix");
    m_uniformAmbientLight = glGetUniformLocation(m_program, "AmbientLight");
    m_uniformSpecularLight = glGetUniformLocation(m_program, "SpecularLight");
    m_uniformShininess = glGetUniformLocation(m_program, "Shininess");
    
    // Generate VBOs for surface
    m_surface = new Torus(1.8f, 0.5f);
    
    vector<float> vertices;
    m_surface->GenerateVertices(vertices, VertexFlagsColors | VertexFlagsNormals | VertexFlagsLightDirection);
    
    vector<unsigned short> indices;
    m_surface->GenerateTriangleIndices(indices);
    m_surfaceIndexCount = indices.size();
    
    glGenBuffers(1, &m_surfaceVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_surfaceIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_surfaceIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

Renderer2::~Renderer2()
{
    delete m_rotator;
    delete m_surface;
}

void Renderer2::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set viewport
    glViewport(0, 0, m_surfaceSize.x, m_surfaceSize.y);
    
    // Enable depth
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    // Apply frustum
    float h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
    
    // Apply modelview
    mat4 translation = mat4::Translate(0.0f, 0.0f, -7.0f);
    mat4 rotation = m_rotator->GetOrientation().ToMatrix();
    mat4 modelview = rotation * translation;
    
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    glUniformMatrix3fv(m_uniformNormalMatrix, 1, GL_FALSE, modelview.ToMat3().Pointer());
    
    // Set lighting parameters
    glUniform3f(m_uniformAmbientLight, 0.0f, 0.0f, 0.0f);
    glUniform3f(m_uniformSpecularLight, 0.5f, 0.5f, 0.5f);
    glUniform1f(m_uniformShininess, 50.0f);
    
    // Draw surface
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribSourceColor);
    glEnableVertexAttribArray(m_attribNormal);
    glEnableVertexAttribArray(m_attribLightDirection);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribSourceColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    glVertexAttribPointer(m_attribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Color)));
    glVertexAttribPointer(m_attribLightDirection, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Color) + sizeof(Vertex::Normal)));
    
    glDrawElements(GL_TRIANGLES, m_surfaceIndexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribLightDirection);
    glDisableVertexAttribArray(m_attribNormal);
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribSourceColor);
}

void Renderer2::OnFingerDown(ivec2 location)
{
    m_rotator->Start(location);
}

void Renderer2::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    m_rotator->Move(newLocation);
}

void Renderer2::OnFingerUp(ivec2 location)
{
    m_rotator->End(location);
}