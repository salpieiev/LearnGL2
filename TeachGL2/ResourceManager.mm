//
//  ResourceManager.mm
//  TeachGL
//
//  Created by Sergey Alpeev on 1/9/13.
//  Copyright (c) 2013 Brisk Mobile Inc. All rights reserved.
//

#include "ResourceManager.h"



TextureDescription ResourceManager::LoadPngImage(const string &fileName)
{
    NSString *file = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *filePath = [resourcePath stringByAppendingPathComponent:file];
    
    UIImage *image = [UIImage imageWithContentsOfFile:filePath];
    CGImageRef cgImage = image.CGImage;
    
    TextureDescription description;
    description.size.x = CGImageGetWidth(cgImage);
    description.size.y = CGImageGetHeight(cgImage);
    description.imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
    description.bitsPerComponent = CGImageGetBitsPerComponent(cgImage);
    
    BOOL hasAlpha = CGImageGetAlphaInfo(cgImage) != kCGImageAlphaNone;
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(cgImage);
    
    CGColorSpaceModel colorSpaceModel = CGColorSpaceGetModel(colorSpace);
    switch (colorSpaceModel)
    {
        case kCGColorSpaceModelMonochrome:
        {
            description.format = hasAlpha ? TextureFormatGrayAlpha : TextureFormatGray;
            break;
        }
        case kCGColorSpaceModelRGB:
        {
            description.format  = hasAlpha ? TextureFormatRGBA : TextureFormatRGB;
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