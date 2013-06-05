//
//  Renderer16.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 05.06.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "RenderingEngine.h"



class Renderer16 : public RenderingEngine
{
public:
    Renderer16(int width, int height);
    ~Renderer16();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    void BuildTextureProgram();
    void LoadTexture();
    void DrawTexture() const;
    
    GLuint m_textureProgram;
    
    GLuint m_backgroundTexture;
    
    GLint m_attribTexturePosition;
    GLint m_attribTextureCoord;
};
