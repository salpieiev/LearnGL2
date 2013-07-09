//
//  Renderer18.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 22.06.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer18.h"
#include "ParametricSurface.h"
#include "Shaders/ProjectiveTexturingLightingShader.vsh"
#include "Shaders/ProjectiveTexturingLightingShader.fsh"



struct Vertex
{
    vec3 Position;
    vec4 Color;
    vec3 Normal;
};



Renderer18::Renderer18(int width, int height): RenderingEngine(width, height)
{
    BuildLightingProgram();
    GenerateSurfaceBuffers();
    LoadProjectorTexture();
    SetupLightingUniforms();
}

Renderer18::~Renderer18()
{
    
}

void Renderer18::Render() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DrawSurface();
}

void Renderer18::OnFingerDown(ivec2 location)
{
    
}

void Renderer18::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    
}

void Renderer18::OnFingerUp(ivec2 location)
{
    
}

void Renderer18::BuildLightingProgram()
{
    m_lightingProgram = BuildProgram(ProjectiveTexturingLightingVertexShader, ProjectiveTexturingLightingFragmentShader);
    glUseProgram(m_lightingProgram);
    
    m_attribLightingPosition = glGetAttribLocation(m_lightingProgram, "a_position");
    m_attribLightingColor = glGetAttribLocation(m_lightingProgram, "a_color");
    m_attribLightingNormal = glGetAttribLocation(m_lightingProgram, "a_normal");
    
    m_uniformLightingProjection = glGetUniformLocation(m_lightingProgram, "u_projection");
    m_uniformLightingModelview = glGetUniformLocation(m_lightingProgram, "u_modelview");
    m_uniformLightingProjectorProjection = glGetUniformLocation(m_lightingProgram, "u_projectorProjection");
    m_uniformLightingProjectorModelview = glGetUniformLocation(m_lightingProgram, "u_projectorModelview");
    m_uniformLightingNormalMatrix = glGetUniformLocation(m_lightingProgram, "u_normalMatrix");
    m_uniformLightingBiasMatrix = glGetUniformLocation(m_lightingProgram, "u_biasMatrix");
    m_uniformLightingLightPosition = glGetUniformLocation(m_lightingProgram, "u_lightPosition");
    m_uniformLightingProjectorPosition = glGetUniformLocation(m_lightingProgram, "u_projectorPosition");
}

void Renderer18::GenerateSurfaceBuffers()
{
//    Cone surface = Cone(5.0f, 1.8f);
//    Cylinder surface = Cylinder(3.0f, 0.5f);
//    Sphere surface = Sphere(1.5f);
//    Torus surface = Torus(1.8f, 0.5f);
//    TrefoilKnot surface = TrefoilKnot(2.5f);
    MobiusStrip surface = MobiusStrip(1.5f);
//    KleinBottle surface = KleinBottle(0.3f);
    
    vector<GLfloat> vertices;
    surface.GenerateVertices(vertices, VertexFlagsColors | VertexFlagsNormals);
    
    vector<GLushort> indices;
    surface.GenerateTriangleIndices(indices);
    m_surfaceIndexCount = indices.size();
    
    glGenBuffers(1, &m_surfaceVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_surfaceVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
    
    glGenBuffers(1, &m_surfaceIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_surfaceIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);
}

void Renderer18::SetupLightingUniforms() const
{
    vec3 lightPosition = vec3(2.5f, 2.5f, 10.0f);
    glUniform3fv(m_uniformLightingLightPosition, 1, &lightPosition.x);
    
#warning Projector Position
    vec3 projectorPosition = vec3(-2.5f, -2.5f, 3.0f);
    glUniform3fv(m_uniformLightingProjectorPosition, 1, &projectorPosition.x);
    
    float h = 4.0f * m_surfaceSize.y / m_surfaceSize.x;
#warning Projection Matrix
    mat4 projection = mat4::Frustum(-2.0f, 2.0f, -h / 2.0f, h / 2.0f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformLightingProjection, 1, GL_FALSE, projection.Pointer());
    
    // Projective matrices
    // Projection matrix
    mat4 projectiveProjection = mat4::Frustum(-0.5f, 0.5f, -0.5f, 0.5f, 4.0f, 10.0f);
    glUniformMatrix4fv(m_uniformLightingProjectorProjection, 1, GL_FALSE, projectiveProjection.Pointer());
    
    // View matrix
    vec3 look = -lightPosition.Normalized();
    vec3 right = vec3(0.0f, 0.0f, 1.0f).Cross(look);
    vec3 up = look.Cross(right);
    vec3 offset;
    offset.x = right.Dot(-lightPosition);
    offset.y = up.Dot(-lightPosition);
    offset.z = look.Dot(-lightPosition);
    
    mat3 projectiveView3;
    projectiveView3.x = right;
    projectiveView3.y = up;
    projectiveView3.z = look;
    projectiveView3 = projectiveView3.Transposed();
    
    mat4 projectiveView4(projectiveView3);
    projectiveView4.w = vec4(offset, 1.0f);
    projectiveView4 = projectiveView4.Transposed();
    glUniformMatrix4fv(m_uniformLightingProjectorModelview, 1, GL_FALSE, projectiveView4.Pointer());
    
    // Bias matrix
    mat3 biasMatrix;
    biasMatrix.x = vec3(0.5f, 0.0f, 0.5f);
    biasMatrix.y = vec3(0.0f, -0.5f, 0.5f);
    biasMatrix.z = vec3(0.0f, 0.0f, 1.0f);
    glUniformMatrix3fv(m_uniformLightingBiasMatrix, 1, GL_FALSE, biasMatrix.Pointer());
}

void Renderer18::LoadProjectorTexture()
{
    glGenTextures(1, &m_projectorTexture);
    glBindTexture(GL_TEXTURE_2D, m_projectorTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    SetPngPOTTexture("tile_floor.png");
}

void Renderer18::DrawSurface() const
{
    glUseProgram(m_lightingProgram);
    glViewport(0, 0, m_surfaceSize.x, m_surfaceSize.y);
    glEnable(GL_DEPTH_TEST);
    
    mat4 modelview;
    modelview = modelview.Translate(0, 0, -5.5f);
    
    mat3 normalMatrix = modelview.ToMat3();
    
    glUniformMatrix4fv(m_uniformLightingModelview, 1, GL_FALSE, modelview.Pointer());
    glUniformMatrix3fv(m_uniformLightingNormalMatrix, 1, GL_FALSE, normalMatrix.Pointer());
    
    glEnableVertexAttribArray(m_attribLightingPosition);
    glEnableVertexAttribArray(m_attribLightingColor);
    glEnableVertexAttribArray(m_attribLightingNormal);
    
    glVertexAttribPointer(m_attribLightingPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribLightingColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position)));
    glVertexAttribPointer(m_attribLightingNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Color)));
    
    glDrawElements(GL_TRIANGLES, m_surfaceIndexCount, GL_UNSIGNED_SHORT, NULL);
    
    glDisableVertexAttribArray(m_attribLightingPosition);
    glDisableVertexAttribArray(m_attribLightingColor);
    glDisableVertexAttribArray(m_attribLightingNormal);
}

















