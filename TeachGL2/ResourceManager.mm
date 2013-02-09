//
//  ResourceManager.mm
//  TeachGL
//
//  Created by Sergey Alpeev on 1/9/13.
//  Copyright (c) 2013 Brisk Mobile Inc. All rights reserved.
//

#include "ResourceManager.h"
#include "PVRTTexture.h"



TextureDescription::TextureDescription()
{
    texFormat = TextureFormatNone;
    bitsPerComponent = 0;
    mipCount = 0;
    texSize = ivec2(0, 0);
    imageData = NULL;
    hasPVRHeader = false;
}

TextureDescription::~TextureDescription()
{
    if (imageData) CFRelease(imageData);
}

void TextureDescription::SetTexFormat(TextureFormat format)
{
    texFormat = format;
}

TextureFormat TextureDescription::GetTexFormat() const
{
    return texFormat;
}

void TextureDescription::SetBitsPerComponent(int bits)
{
    bitsPerComponent = bits;
}

int TextureDescription::GetBitsPerComponent() const
{
    return bitsPerComponent;
}

void TextureDescription::SetMipCount(int count)
{
    mipCount = count;
}

int TextureDescription::GetMipCount() const
{
    return mipCount;
}

void TextureDescription::SetTexSize(ivec2 size)
{
    texSize = size;
}

ivec2 TextureDescription::GetTexSize() const
{
    return texSize;
}

void TextureDescription::SetTexData(CFDataRef data)
{
    imageData = data;
}

void * TextureDescription::GetTexHeader() const
{
    if (!hasPVRHeader)
        return NULL;
    
    void *data = (void *)CFDataGetBytePtr(imageData);
    return data;
}

void * TextureDescription::GetTexData() const
{
    void *data = (void *)CFDataGetBytePtr(imageData);
    
    if (!hasPVRHeader)
        return data;
    
    PVR_Texture_Header *header = (PVR_Texture_Header *)data;
    char *charData = (char *)data;
    unsigned int headerSize = header->dwHeaderSize;
    charData += headerSize;
    return charData;
}

void TextureDescription::SetHasPVRHeader(bool hasHeader)
{
    hasPVRHeader = hasHeader;
}

bool TextureDescription::GeetHasPVRHeader() const
{
    return hasPVRHeader;
}



TextureDescription ResourceManager::LoadPngImage(const string &fileName)
{
    NSString *file = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *filePath = [resourcePath stringByAppendingPathComponent:file];
    
    UIImage *image = [UIImage imageWithContentsOfFile:filePath];
    CGImageRef cgImage = image.CGImage;
    
    TextureDescription description;
    ivec2 imageSize = ivec2(CGImageGetWidth(cgImage), CGImageGetHeight(cgImage));
    description.SetTexSize(imageSize);
    description.SetTexData(CGDataProviderCopyData(CGImageGetDataProvider(cgImage)));
    description.SetBitsPerComponent(CGImageGetBitsPerComponent(cgImage));
    
    BOOL hasAlpha = CGImageGetAlphaInfo(cgImage) != kCGImageAlphaNone;
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(cgImage);
    
    CGColorSpaceModel colorSpaceModel = CGColorSpaceGetModel(colorSpace);
    switch (colorSpaceModel)
    {
        case kCGColorSpaceModelMonochrome:
        {
            description.SetTexFormat(hasAlpha ? TextureFormatGrayAlpha : TextureFormatGray);
            break;
        }
        case kCGColorSpaceModelRGB:
        {
            description.SetTexFormat(hasAlpha ? TextureFormatRGBA : TextureFormatRGB);
            break;
        }
        default:
        {
            assert(!"Unsupported color space");
            break;
        }
    }
    
    return description;
}

TextureDescription ResourceManager::LoadPVRImage(const string &fileName)
{
    NSString *file = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *filePath = [resourcePath stringByAppendingPathComponent:file];
    NSData *fileData = [NSData dataWithContentsOfFile:filePath];
    
    TextureDescription description;
    description.SetTexData((CFDataRef)CFBridgingRetain(fileData));
    description.SetHasPVRHeader(true);
    
    PVR_Texture_Header *textureHeader = (PVR_Texture_Header *)description.GetTexHeader();
    bool hasAlpha = textureHeader->dwAlphaBitMask;
    
    unsigned int pixelType = (textureHeader->dwpfFlags & PVRTEX_PIXELTYPE);
    switch (pixelType)
    {
        /*case OGL_RGB_565:
            description.Format = TextureFormat565;
            break;
        case OGL_RGBA_5551:
            description.Format = TextureFormat5551;
            break;
        case OGL_RGBA_4444:
            description.Format = TextureFormatRgba;
            description.BitsPerComponent = 4;
            break;*/
        case OGL_PVRTC2:
        {
            TextureFormat texFormat = hasAlpha ? TextureFormatPVRTC_RGBA2 : TextureFormatPVRTC_RGB2;
            description.SetTexFormat(texFormat);
            break;
        }
        case OGL_PVRTC4:
        {
            TextureFormat texFormat = hasAlpha ? TextureFormatPVRTC_RGBA4 : TextureFormatPVRTC_RGB4;
            description.SetTexFormat(texFormat);
            break;
        }
        default:
            assert(!"Unsupported PVR image");
            break;
    }
    
    ivec2 texSize = ivec2(textureHeader->dwWidth, textureHeader->dwHeight);
    description.SetTexSize(texSize);
    description.SetMipCount(textureHeader->dwMipMapCount);
    return description;
}


