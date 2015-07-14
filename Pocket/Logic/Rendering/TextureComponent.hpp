//
//  TextureComponent.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/24/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameComponent.hpp"
#include "Texture.hpp"

namespace Pocket {
    Component(TextureComponent)
    public:
        TextureComponent();
        ~TextureComponent();
        
        Pocket::Texture& Texture();
        void Reset();
    protected:
        void Clone(const TextureComponent& source);
        
    private:
        class Pocket::Texture texture;
    };
}