//
//  ResourceManager.mm
//  TeachGL
//
//  Created by Sergey Alpeev on 1/9/13.
//  Copyright (c) 2013 Brisk Mobile Inc. All rights reserved.
//

#include "ResourceManager.h"



TextureDescription::TextureDescription()
{
    imageData = NULL;
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

void * TextureDescription::GetTexData() const
{
    return (void *)CFDataGetBytePtr(imageData);
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
            assert("Unsupported color space");
            break;
        }
    }
    
    return description;
}