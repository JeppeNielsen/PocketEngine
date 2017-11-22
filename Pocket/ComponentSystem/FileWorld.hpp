//
//  FileWorld.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 13/11/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <map>
#include <functional>
#include <vector>

namespace Pocket {
    class GameWorld;
    class GameObject;
    class FileWorld {
    public:
        void AddGameWorld(GameWorld& world);
        void FindRoots(const std::string& path, const std::vector<std::string>& extensions);
        
        std::function<void(GameObject* root)> OnRootCreated;
        std::function<void(GameObject* child)> OnChildCreated;
        
        const std::map<std::string, std::string>& Paths();
        
        std::string GetPathFromGuid(const std::string& guid);
        
    private:
        std::map<std::string, std::string> guidToPath;
    };
}
