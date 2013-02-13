//
//  Renderer6.cpp
//  TeachGL2
//
//  Created by Sergey Alpeev on 1/21/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer6.h"
#include "Shaders/VertexSkinningShader.vsh"
#include "Shaders/VertexSkinningShader.fsh"



const vec3 Eye(0.5f, 1, 62);
const vec3 Target(0.5f, 1, 0);
const vec3 Up(0, 1, 0);

struct Vertex
{
    vec3 Position;
    vec4 Color;
    vec3 Normal;
    vec2 BoneWeights;
    float Padding0;
    unsigned short boneIndices;
    unsigned short Padding1;
};

typedef vector<Vertex> VertexList;
typedef vector<GLushort> IndexList;

struct Skeleton
{
    IndexList Indices;
    VertexList Vertices;
};

struct SkinnedFigure
{
    GLuint IndexBuffer;
    GLuint VertexBuffer;
    MatrixList Matrices;
};



Renderer6::Renderer6(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    m_rotator = new Rotator(m_surfaceSize);
    
    m_program = BuildProgram(VertexSkinningVertexShader, VertexSkinningFragmentShader);
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
    
    
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribSourceColor);
    glEnableVertexAttribArray(m_attribNormal);
    
    // Create surface
//    m_surface = new Cone(5.0f, 1.8f);
//    m_surface = new Sphere(2.0f);
//    m_surface = new Torus(1.8f, 0.5f);
    m_surface = new TrefoilKnot(3.0f);
//    m_surface = new MobiusStrip(1.5f);
//    m_surface = new KleinBottle(0.3f);
    
    vector<float> vertices;
    m_surface->GenerateVertices(vertices, VertexFlagsColors | VertexFlagsNormals);
    
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
    
    // Setup uniforms
    glUniform3f(m_uniformLightPosition, 0.25f, 0.25f, 1.0f);
    glUniform3f(m_uniformAmbientLight, 0.04f, 0.04f, 0.04f);
    glUniform3f(m_uniformSpecularLight, 0.5f, 0.5f, 0.5f);
    glUniform1f(m_uniformShininess, 50);
}

Renderer6::~Renderer6()
{
    delete m_rotator;
    delete m_surface;
}

void Renderer6::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mat4 orientation = m_rotator->GetOrientation().ToMatrix();
    mat4 translation = mat4::Translate(0.0f, 0.0f, -7.0f);
    mat4 modelview = orientation * translation;
    mat3 normalMatrix = modelview.ToMat3();
    
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    glUniformMatrix3fv(m_uniformNormalMatrix, 1, GL_FALSE, normalMatrix.Pointer());
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribSourceColor, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    glVertexAttribPointer(m_attribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Color)));
    
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
}

void Renderer6::OnFingerDown(ivec2 location)
{
    m_rotator->Start(location);
}

void Renderer6::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    m_rotator->Move(newLocation);
}

void Renderer6::OnFingerUp(ivec2 location)
{
    m_rotator->End(location);
}

void Renderer6::ComputeMatrices(const Skeleton &skeleton, const MatrixList &matrices)
{
    mat4 modelview = mat4::LookAt(Eye, Target, Up);
    
    float x = 0.0f;
    IndexList::const_iterator lineIndex = skeleton.Indices.begin();
    
}


