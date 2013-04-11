//
//  Renderer5.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer5.h"
#include "Shaders/SphereMapShader.vsh"
#include "Shaders/SphereMapShader.fsh"



struct Vertex
{
    vec3 Position;
    vec3 Normal;
};



Renderer5::Renderer5(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    m_rotator = new Rotator(m_surfaceSize);
    
    m_program = BuildProgram(SphereMapVertexShader, SphereMapFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "Position");
    m_attribNormal = glGetAttribLocation(m_program, "Normal");
    
    m_uniformProjection = glGetUniformLocation(m_program, "Projection");
    m_uniformModelview = glGetUniformLocation(m_program, "Modelview");
    m_uniformNormalMatrix = glGetUniformLocation(m_program, "NormalMatrix");
    m_uniformEyePosition = glGetUniformLocation(m_program, "EyePosition");
    
    // Create surface
//    m_surface = new Cone(5.0f, 1.8f);
    m_surface = new Sphere(2.0f);
//    m_surface = new Torus(1.8f, 0.5f);
//    m_surface = new TrefoilKnot(3.0f);
//    m_surface = new MobiusStrip(1.5f);
//    m_surface = new KleinBottle(0.3f);
    
    vector<float> vertices;
    m_surface->GenerateVertices(vertices, VertexFlagsNormals);
    
    vector<unsigned short> indices;
    m_surface->GenerateTriangleIndices(indices);
    m_indexCount = indices.size();
    
    // Generate vertex buffer
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    
    // Generate index buffer
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), &indices[0], GL_STATIC_DRAW);
    
    // Set frustum
    GLfloat h = 4 * height / width;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
    
    // Generate texture
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    SetPngPOTTexture("sphere_map.png");
    
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Renderer5::~Renderer5()
{
    delete m_rotator;
    delete m_surface;
}

void Renderer5::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mat4 orientation = m_rotator->GetOrientation().ToMatrix();
    mat4 translation = mat4::Translate(0.0f, 0.0f, -7.0f);
    mat4 modelview = orientation * translation;
    mat3 normalMatrix = modelview.ToMat3();
    
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    glUniformMatrix3fv(m_uniformNormalMatrix, 1, GL_FALSE, normalMatrix.Pointer());
    
    // Setup uniforms
    vec3 eyeWorldSpace = vec3(0.0f, 0.0f, -10.0f);
    vec3 eyeObjectSpace = normalMatrix * eyeWorldSpace;
    
    glUniform3fv(m_uniformEyePosition, 1, eyeObjectSpace.Pointer());
    
    // Enable Attributes
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribNormal);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    
    // Disable attributes
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribNormal);
}

void Renderer5::OnFingerDown(ivec2 location)
{
    m_rotator->Start(location);
}

void Renderer5::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    m_rotator->Move(newLocation);
}

void Renderer5::OnFingerUp(ivec2 location)
{
    m_rotator->End(location);
}


