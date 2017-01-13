//
//  FontTextureSystem.cpp
//  TestFreeType
//
//  Created by Jeppe Nielsen on 13/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "FontTextureSystem.hpp"
#include "OpenGL.hpp"

void FontTextureSystem::Update(float dt) {
    for(auto o : Objects()) {
        Font* font = o->GetComponent<Font>();
        if (font->UpdateBuffer()) {
            TextureComponent* texture = o->GetComponent<TextureComponent>();
            texture->Texture().CreateFromBuffer(font->buffer, font->bufferWidth, font->bufferHeight, GL_LUMINANCE);
        }
    }
}
