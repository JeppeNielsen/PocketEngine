//
//  FontTextureSystem.cpp
//  TestFreeType
//
//  Created by Jeppe Nielsen on 13/01/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "FontTextureSystem.hpp"
#include "OpenGL.hpp"

using namespace Pocket;

void FontTextureSystem::Update(float dt) {
    for(auto o : Objects()) {
        Font* font = o->GetComponent<Font>();
        if (font->IsDirty()) {
            TextureComponent* texture = o->GetComponent<TextureComponent>();
            font->UpdateBuffer(texture->Texture());
        }
    }
}
