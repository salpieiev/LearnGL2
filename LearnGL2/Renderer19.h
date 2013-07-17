//
//  Renderer19.h
//  LearnGL2
//
//  Created by Sergey Alpeev on 17.07.13.
//  Copyright (c) 2013 Sergey Alpeev. All rights reserved.
//

#include "RenderingEngine.h"



class Renderer19 : public RenderingEngine
{
public:
    Renderer19(int width, int height);
    ~Renderer19();
    
    void Render() const;
    void OnFingerDown(ivec2 location);
    void OnFingerMove(ivec2 oldLocation, ivec2 newLocation);
    void OnFingerUp(ivec2 location);
    
private:
    GLint m_attribPosition;
    GLint m_attribTexCoord;
};
