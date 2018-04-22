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
#include "Mesh.hpp"

using namespace Pocket;

void AssetManager::Initialize() {
    root->CreateSystem<AssetLoaderSystem<TextureComponent>>();
    root->CreateSystem<AssetLoaderSystem<ShaderComponent>>();
    root->CreateSystem<AssetLoaderSystem<Font>>();
    root->CreateSystem<AssetLoaderSystem<Mesh>>();
}

void AssetManager::SetFileWatcher(Pocket::FileSystemWatcher *watcher) {
    root->CreateSystem<AssetLoaderSystem<TextureComponent>>()->SetFileWatcher(watcher);
    root->CreateSystem<AssetLoaderSystem<ShaderComponent>>()->SetFileWatcher(watcher);
    root->CreateSystem<AssetLoaderSystem<Font>>()->SetFileWatcher(watcher);
    root->CreateSystem<AssetLoaderSystem<Mesh>>()->SetFileWatcher(watcher);
}

void AssetManager::CreateSubSystems(Pocket::GameStorage &storage) {
    storage.AddSystemType<AssetLoaderSystem<TextureComponent>>();
    storage.AddSystemType<AssetLoaderSystem<ShaderComponent>>();
    storage.AddSystemType<AssetLoaderSystem<Font>>();
    storage.AddSystemType<AssetLoaderSystem<Mesh>>();
}
