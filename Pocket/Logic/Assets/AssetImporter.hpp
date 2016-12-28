//
//  AssetImporter.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 28/12/2016.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <functional>

namespace Pocket {

    class GameObject;

    struct AssetImporter {
        std::string extension;
        std::function<void(GameObject* object)> OnCreated;
    };
}
