//
//  AssetLoaderSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 22/12/2016.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include "AssetLoader.hpp"
#include "FileSystemWatcher.hpp"
#include <iostream>

namespace Pocket {
    template<typename T>
    class AssetLoaderSystem : public GameSystem<AssetLoader, T> {
    public:
        AssetLoaderSystem() : watcher(0) { }
    
        void SetFileWatcher(FileSystemWatcher* watcher) {
            this->watcher = watcher;
            watcher->FileModified.Bind(this, &AssetLoaderSystem<T>::FileModified);
        }
    private:
        FileSystemWatcher* watcher;
    protected:
    
        void FileModified(std::string path) {
            for(auto o : this->Objects()) {
                if (o->template GetComponent<AssetLoader>()->pathLoaded == path) {
                    LoadAsset(o);
                }
            }
        }
        
        void Destroy() override {
            if (watcher) {
                watcher->FileModified.Unbind(this, &AssetLoaderSystem<T>::FileModified);
            }
        }
    
        void ObjectAdded(GameObject* object) override {
            LoadAsset(object);
        }
        
        void LoadAsset(GameObject* object) {
            AssetLoader* assetLoader = object->GetComponent<AssetLoader>();
            GameObject* ownerObject = object->GetComponentOwner<T>();
            std::string rootFilename = ownerObject->TryGetScenePath();
            if (rootFilename == ownerObject->RootGuid()) {
                assetLoader->pathLoaded = rootFilename + "-asset";
                T* asset = object->GetComponent<T>();
                asset->LoadAsset(assetLoader->pathLoaded);
            } else {
                auto pos = rootFilename.rfind(".");
                if (pos != std::string::npos) {
                    std::string fileNameWithoutExtension = rootFilename.substr(0, pos);
                    std::cout << "fileNameWithoutExtension : " << fileNameWithoutExtension << std::endl;
                    
                    T* asset = object->GetComponent<T>();
                    asset->LoadAsset(fileNameWithoutExtension);
                    assetLoader->pathLoaded = fileNameWithoutExtension;
                }
            }
        }
    };
}
