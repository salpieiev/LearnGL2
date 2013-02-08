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



enum TextureFormat
{
    TextureFormatGray,
    TextureFormatGrayAlpha,
    TextureFormatRGB,
    TextureFormatRGBA
};



class TextureDescription
{
public:
    TextureDescription() {}
    ~TextureDescription()
    {
        if (imageData) CFRelease(imageData);
    }
    
    void * GetImageData() const
    {
        return (void *)CFDataGetBytePtr(imageData);
    }
    
    TextureFormat format;
    int bitsPerComponent;
    ivec2 size;
    CFDataRef imageData;
};



class ResourceManager
{
public:
    TextureDescription LoadPngImage(const string &fileName);
};