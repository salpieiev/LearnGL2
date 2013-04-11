//
//  ResourceManager.h
//  LearnGL
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
    
    void SetTexOriginalSize(ivec2 texSize);
    ivec2 GetTexOriginalSize() const;
    
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
    ivec2 originalSize;
    CFDataRef imageData;
    bool hasPVRHeader;
};



class ResourceManager
{
public:
    TextureDescription LoadPngPOTImage(const string &fileName) const;
    TextureDescription LoadPngNPOTImage(const string &fileName) const;
    TextureDescription LoadPVRImage(const string &fileName) const;
    
    string ReadTextFile(const string &fileName) const;
    
private:
    unsigned int NextPOT(unsigned int n) const;
};