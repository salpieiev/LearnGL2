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
    TextureDescription();
    ~TextureDescription();
    
    void SetTexFormat(TextureFormat format);
    TextureFormat GetTexFormat() const;
    
    void SetBitsPerComponent(int bits);
    int GetBitsPerComponent() const;
    
    void SetTexSize(ivec2 texSize);
    ivec2 GetTexSize() const;
    
    void SetTexData(CFDataRef data);
    void * GetTexData() const;
    
private:
    TextureFormat texFormat;
    int bitsPerComponent;
    ivec2 texSize;
    CFDataRef imageData;
};



class ResourceManager
{
public:
    TextureDescription LoadPngImage(const string &fileName);
};