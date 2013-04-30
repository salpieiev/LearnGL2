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
    vec3 Normals;
    vec2 TexCoords;
    vec3 Tangents;
};



Renderer12::Renderer12(int width, int height): RenderingEngine(width, height)
{
    m_rotator = new Rotator(m_surfaceSize);
    
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    
    PrepareProgram();
    GenerateBuffers();
    SetupUniforms();
    LoadTexture();
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
    m_attribNormal = glGetAttribLocation(m_program, "a_normal");
    m_attribTangent = glGetAttribLocation(m_program, "a_tangent");
    m_attribTexCoord = glGetAttribLocation(m_program, "a_texCoord");
    
    m_uniformProjection = glGetUniformLocation(m_program, "u_projection");
    m_uniformModelview = glGetUniformLocation(m_program, "u_modelview");
    m_uniformNormalMatrix = glGetUniformLocation(m_program, "u_normalMatrix");
    m_uniformAmbientMaterial = glGetUniformLocation(m_program, "u_ambientMaterial");
    m_uniformDiffuseMaterial = glGetUniformLocation(m_program, "u_diffuseMaterial");
    m_uniformSpecularMaterial = glGetUniformLocation(m_program, "u_specularMaterial");
    m_uniformLightVector = glGetUniformLocation(m_program, "u_lightVector");
    m_uniformEyeVector = glGetUniformLocation(m_program, "u_eyeVector");
    m_uniformShininess = glGetUniformLocation(m_program, "u_shininess");
}

void Renderer12::GenerateBuffers()
{
    Sphere sphere(2.5f, vec2(1.0f, 1.0f));
    
    vector<float> vertices;
    sphere.GenerateVertices(vertices, VertexFlagsTexCoords | VertexFlagsNormals | VertexFlagsTangents);
    
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

void Renderer12::SetupUniforms()
{
    float h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformProjection, 1, GL_FALSE, projection.Pointer());
    
    glUniform3f(m_uniformAmbientMaterial, 0.04f, 0.04f, 0.04f);
    glUniform3f(m_uniformDiffuseMaterial, 0.75f, 0.75f, 0.75f);
    glUniform3f(m_uniformSpecularMaterial, 0.5f, 0.5f, 0.5f);
    glUniform1f(m_uniformShininess, 50);
}

void Renderer12::LoadTexture()
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
//    SetPngPOTTexture("NormalMapEarth.png");
    SetPVRTexture("NormalMapEarth.pvr");
    
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Renderer12::DrawSphere() const
{
    mat4 model = m_rotator->GetOrientation().ToMatrix();
    mat4 view = mat4::Translate(0.0f, 0.0f, -7.0f);
    mat4 modelview = model * view;
    
    vec4 lightWorldSpace = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    vec4 lightObjectSpace = model * lightWorldSpace;
    
    vec4 eyeWorldSpace(0.0f, 0.0f, 1.0f, 1.0f);
    vec4 eyeObjectSpace = model * eyeWorldSpace;
    
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    glUniformMatrix3fv(m_uniformNormalMatrix, 1, GL_FALSE, modelview.ToMat3().Pointer());
    glUniform3fv(m_uniformLightVector, 1, lightObjectSpace.Pointer());
    glUniform3fv(m_uniformEyeVector, 1, eyeObjectSpace.Pointer());
    
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribNormal);
    glEnableVertexAttribArray(m_attribTangent);
    glEnableVertexAttribArray(m_attribTexCoord);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position)));
    glVertexAttribPointer(m_attribTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Normals)));
    glVertexAttribPointer(m_attribTangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Normals) + sizeof(Vertex::TexCoords)));
    
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribNormal);
    glDisableVertexAttribArray(m_attribTangent);
    glDisableVertexAttribArray(m_attribTexCoord);
}




