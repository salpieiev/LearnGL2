//
//  Renderer11.cpp
//  LearnGL2
//
//  Created by Sergey Alpeev on 4/22/13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "Renderer11.h"
#include "Shaders/RenderToTextureShader.vsh"
#include "Shaders/RenderToTextureShader.fsh"



struct Vertex
{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoord;
};



Renderer11::Renderer11(int width, int height): RenderingEngine(width, height)
{
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    m_rotator = new Rotator(m_surfaceSize);
    
    m_program = BuildProgram(RenderToTextureVertexShader, RenderToTextureFragmentShader);
    glUseProgram(m_program);
    
    m_attribPosition = glGetAttribLocation(m_program, "Position");
    m_attribSourceColor = glGetAttribLocation(m_program, "SourceColor");
    m_attribNormal = glGetAttribLocation(m_program, "Normal");
    m_attribTextureCoord = glGetAttribLocation(m_program, "TextureCoordIn");
    
    m_uniformProjection = glGetUniformLocation(m_program, "Projection");
    m_uniformModelview = glGetUniformLocation(m_program, "Modelview");
    m_uniformNormalMatrix = glGetUniformLocation(m_program, "NormalMatrix");
    m_uniformLightPosition = glGetUniformLocation(m_program, "LightPosition");
    m_uniformAmbientLight = glGetUniformLocation(m_program, "AmbientLight");
    m_uniformSpecularLight = glGetUniformLocation(m_program, "SpecularLight");
    m_uniformShininess = glGetUniformLocation(m_program, "Shininess");
    
    // Create surface
    //    m_surface = new Cone(5.0f, 1.8f);
    //    m_surface = new Sphere(2.0f);
    //    m_surface = new Torus(1.8f, 0.5f);
    //    m_surface = new TrefoilKnot(3.0f);
    m_surface = new MobiusStrip(1.5f);
    //    m_surface = new KleinBottle(0.3f);
    
    vector<float> vertices;
    m_surface->GenerateVertices(vertices, VertexFlagsNormals | VertexFlagsTexCoords);
    
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
    glVertexAttrib4f(m_attribSourceColor, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform3f(m_uniformLightPosition, 0.25f, 0.25f, 1.0f);
    glUniform3f(m_uniformAmbientLight, 0.04f, 0.04f, 0.04f);
    glUniform3f(m_uniformSpecularLight, 0.5f, 0.5f, 0.5f);
    glUniform1f(m_uniformShininess, 50);
    
    // Generate Flag texture
    glGenTextures(1, &m_textureFlag);
    glBindTexture(GL_TEXTURE_2D, m_textureFlag);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    SetPngPOTTexture("Star.png");
    
    glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Renderer11::~Renderer11()
{
    delete m_rotator;
    delete m_surface;
}

void Renderer11::Render() const
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    mat4 orientation = m_rotator->GetOrientation().ToMatrix();
    mat4 translation = mat4::Translate(0.0f, 0.0f, -7.0f);
    mat4 modelview = orientation * translation;
    mat3 normalMatrix = modelview.ToMat3();
    
    glUniformMatrix4fv(m_uniformModelview, 1, GL_FALSE, modelview.Pointer());
    glUniformMatrix3fv(m_uniformNormalMatrix, 1, GL_FALSE, normalMatrix.Pointer());
    
    // Enable Attributes
    glEnableVertexAttribArray(m_attribPosition);
    glEnableVertexAttribArray(m_attribNormal);
    glEnableVertexAttribArray(m_attribTextureCoord);
    
    glVertexAttribPointer(m_attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
    glVertexAttribPointer(m_attribNormal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertex::Position));
    glVertexAttribPointer(m_attribTextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(Vertex::Position) + sizeof(Vertex::Normal)));
    
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, NULL);
    
    // Disable attributes
    glDisableVertexAttribArray(m_attribPosition);
    glDisableVertexAttribArray(m_attribNormal);
    glDisableVertexAttribArray(m_attribTextureCoord);
}

void Renderer11::OnFingerDown(ivec2 location)
{
    m_rotator->Start(location);
}

void Renderer11::OnFingerMove(ivec2 oldLocation, ivec2 newLocation)
{
    m_rotator->Move(newLocation);
}

void Renderer11::OnFingerUp(ivec2 location)
{
    m_rotator->End(location);
    
    GenerateTexture();
}

void Renderer11::GenerateTexture() const
{
    GLuint depthRenderbuffer;
    glGenRenderbuffers(1, &depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
    
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT16, GL_RENDERBUFFER, depthRenderbuffer);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
    {
        glClearColor(0.5f, 1.0f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &depthRenderbuffer);
}



