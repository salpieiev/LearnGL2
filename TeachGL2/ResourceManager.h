//
//  ResourceManager.h
//  TeachGL
//
//  Created by Sergey Alpeev on 1/9/13.
//  Copyright (c) 2013 Brisk Mobile Inc. All rights reserved.
//

#pragma once
#include "Vector.h"
#include <string>
#include <QuartzCore/QuartzCore.h>

using namespace std;



class ResourceManager
{
public:
    void LoadPngImage(const string &fileName);
    void UnloadImage();
    void * GetImageData() const;
    ivec2 GetImageSize() const;
    
private:
    CFDataRef m_imageData;
    ivec2 m_imageSize;
};