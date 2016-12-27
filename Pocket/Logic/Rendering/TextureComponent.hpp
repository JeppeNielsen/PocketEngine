//
//  TextureComponent.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/24/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Texture.hpp"

namespace Pocket {
    struct TextureComponent {
    public:
        TextureComponent();
        ~TextureComponent();
        
        Pocket::Texture& Texture();
        
        void LoadAsset(const std::string& path);
    private:
        class Pocket::Texture texture;
        std::string prevPath;
    };
}