//
//  TextureComponent.cpp
//  PocketEngine
//
//  Created by Jeppe Nielsen on 8/24/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#include "TextureComponent.hpp"

using namespace Pocket;

Texture& TextureComponent::Texture() { return texture; }

TextureComponent::TextureComponent() {}
TextureComponent::~TextureComponent() { texture.Free(); }

void TextureComponent::LoadAsset(const std::string &path) {
    texture.LoadFromFile(path);
}
