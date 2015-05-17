//
//  TextureComponent.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/24/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "TextureComponent.hpp"

using namespace Pocket;

void TextureComponent::Reset() {

}

void TextureComponent::Clone(const Pocket::TextureComponent &source) {
    texture = source.texture;
}

Texture& TextureComponent::Texture() { return texture; }

TextureComponent::TextureComponent() {}
TextureComponent::~TextureComponent() { texture.Free(); }
