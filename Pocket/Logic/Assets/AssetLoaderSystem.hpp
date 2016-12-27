//
//  AssetLoaderSystem.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 22/12/2016.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameSystem.hpp"
#include <iostream>

namespace Pocket {
    template<typename T>
    class AssetLoaderSystem : public GameSystem<T> {
    protected:
        void ObjectAdded(GameObject* object) {
            GameObject* ownerObject = object->GetComponentOwner<T>();
            std::string rootFilename = ownerObject->TryGetRootPath();
            auto pos = rootFilename.rfind(".");
            if (pos != std::string::npos) {
                std::string fileNameWithoutExtension = rootFilename.substr(0, pos);
                std::cout << "fileNameWithoutExtension : " << fileNameWithoutExtension << std::endl;
                
                T* asset = object->GetComponent<T>();
                asset->LoadAsset(fileNameWithoutExtension);
            }
        //    std::cout << "Filename : "<<rootFilename << "pos : " << pos << std::endl;
        }
        
        void ObjectRemoved(GameObject* object) {
            
        }
    };
}
