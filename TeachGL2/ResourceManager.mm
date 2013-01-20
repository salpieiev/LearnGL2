//
//  ResourceManager.mm
//  TeachGL
//
//  Created by Sergey Alpeev on 1/9/13.
//  Copyright (c) 2013 Brisk Mobile Inc. All rights reserved.
//

#include "ResourceManager.h"



void ResourceManager::LoadPngImage(const string &fileName)
{
    NSString *file = [NSString stringWithCString:fileName.c_str() encoding:NSUTF8StringEncoding];
    NSString *resourcePath = [[NSBundle mainBundle] resourcePath];
    NSString *filePath = [resourcePath stringByAppendingPathComponent:file];
    
    UIImage *image = [UIImage imageWithContentsOfFile:filePath];
    CGImageRef cgImage = image.CGImage;
    
    m_imageSize.x = CGImageGetWidth(cgImage);
    m_imageSize.y = CGImageGetHeight(cgImage);
    
    m_imageData = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
}

void ResourceManager::UnloadImage()
{
    m_imageSize = ivec2();
    
    CFRelease(m_imageData);
    m_imageData = NULL;
}

void * ResourceManager::GetImageData() const
{
    return (void *)CFDataGetBytePtr(m_imageData);
}

ivec2 ResourceManager::GetImageSize() const
{
    return m_imageSize;
}