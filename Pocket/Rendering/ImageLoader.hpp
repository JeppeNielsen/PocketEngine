//
//  ImageLoader.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 30/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <functional>

namespace Pocket {
    class ImageLoader {
    public:
        static bool TryLoadImage(const std::string& path, const std::function<void(unsigned char* pixels, int width, int height)>& onLoaded);
        
        static bool TryLoadImageFromData(unsigned char* data, int size, const std::function<void(unsigned char* pixels, int width, int height)>& onLoaded);
        
        static bool SaveTga(const std::string& path, unsigned char* pixels, int width, int height);
    };
}
