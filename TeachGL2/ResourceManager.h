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
    TextureFormatNone,
    TextureFormatGray,
    TextureFormatGrayAlpha,
    TextureFormatRGB,
    TextureFormatRGBA,
    TextureFormatPVRTC_RGB2,
    TextureFormatPVRTC_RGBA2,
    TextureFormatPVRTC_RGB4,
    TextureFormatPVRTC_RGBA4,
    /*TextureFormat565,
    TextureFormat5551*/
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
    
    void SetMipCount(int count);
    int GetMipCount() const;
    
    void SetTexSize(ivec2 texSize);
    ivec2 GetTexSize() const;
    
    void SetTexData(CFDataRef data);
    void * GetTexHeader() const;
    void * GetTexData() const;
    
    void SetHasPVRHeader(bool hasHeader);
    bool GeetHasPVRHeader() const;
    
private:
    TextureFormat texFormat;
    int bitsPerComponent;
    int mipCount;
    ivec2 texSize;
    CFDataRef imageData;
    bool hasPVRHeader;
};



class ResourceManager
{
public:
    TextureDescription LoadPngImage(const string &fileName);
    TextureDescription LoadPVRImage(const string &fileName);
};