//
//  ImageLoader.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 30/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "ImageLoader.hpp"
#include "File.hpp"
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

using namespace Pocket;

bool ImageLoader::TryLoadImage(const std::string &path, const std::function<void (unsigned char *, int, int)> &onLoaded) {
    
    File file;
    if (!file.Load(path)) {
        return false;
    }
    
    return TryLoadImageFromData(file.Data(), (int)file.Size(), onLoaded);
}

bool ImageLoader::TryLoadImageFromData(unsigned char *data, int size, const std::function<void (unsigned char *, int, int)> &onLoaded) {

    int width, height, bpp;
    unsigned char* pixels = stbi_load_from_memory(data, size, &width, &height, &bpp, 4 );
    if (pixels) {
        onLoaded(pixels, width, height);
        return  true;
    }
    return false;
}
