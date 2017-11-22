//
//  ImageHelper.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 20/11/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ImageHelper.hpp"
#include "File.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

using namespace Pocket;

bool ImageHelper::SavePng(const std::string& path, unsigned char* pixels, int width, int height) {
    return stbi_write_png(path.c_str(), width, height, 4, pixels, 0) !=0;
}




unsigned char* ImageHelper::ScaleImage(unsigned char* pixels, int width, int height, int newWidth, int newHeight) {
    unsigned char* outputPixels = new unsigned char[newWidth * newHeight * 4];
    int error = stbir_resize_uint8(pixels, width, height, 0, outputPixels, newWidth, newHeight, 0, 4);
    return outputPixels;
}

