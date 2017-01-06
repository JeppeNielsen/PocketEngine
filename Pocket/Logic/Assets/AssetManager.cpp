//
//  AssetManager.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 22/12/2016.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "AssetManager.hpp"
#include "AssetLoaderSystem.hpp"
#include "TextureComponent.hpp"
#include "ShaderComponent.hpp"

using namespace Pocket;

void AssetManager::Initialize() {
    root->CreateSystem<AssetLoaderSystem<TextureComponent>>();
    root->CreateSystem<AssetLoaderSystem<ShaderComponent>>();
}
