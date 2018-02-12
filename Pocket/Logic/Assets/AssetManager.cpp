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
#include "Font.hpp"

using namespace Pocket;

void AssetManager::SetFileWatcher(Pocket::FileSystemWatcher *watcher) {
    root->GetSystem<AssetLoaderSystem<TextureComponent>>()->SetFileWatcher(watcher);
    root->GetSystem<AssetLoaderSystem<ShaderComponent>>()->SetFileWatcher(watcher);
    root->GetSystem<AssetLoaderSystem<Font>>()->SetFileWatcher(watcher);
}

void AssetManager::CreateSubSystems(Pocket::GameStorage &storage) {
    storage.AddSystemType<AssetLoaderSystem<TextureComponent>>();
    storage.AddSystemType<AssetLoaderSystem<ShaderComponent>>();
    storage.AddSystemType<AssetLoaderSystem<Font>>();
}
