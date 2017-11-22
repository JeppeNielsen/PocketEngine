//
//  ImageHelper.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 20/11/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <functional>

namespace Pocket {
    class ImageHelper {
    public:
        static bool SavePng(const std::string& path, unsigned char* pixels, int width, int height);
        static unsigned char* ScaleImage(unsigned char* pixels, int width, int height, int newWidth, int newHeight);
        
    };
}
