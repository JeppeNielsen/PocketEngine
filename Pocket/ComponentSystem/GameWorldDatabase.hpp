//
//  GameWorldDatabase.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 16/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "GameWorld.hpp"
#include <map>
#include <fstream>

namespace Pocket {
    class GameWorldDatabase {
    public:
        using Paths = std::map<std::string, std::string>;
        void AddPath(const std::string& path);
        std::string GetPath(const std::string& guid);
        void RemoveInvalidPaths();
        bool DoesWorldExists(const std::string& guid);
        void Clear();
        const Paths& GetPaths();
        
        GameObject* FindGameObject(const std::string& guidWorld, const std::string& objectID = "");
        
    private:
        Paths paths;
        std::string ReadGuidFromStream(std::istream &jsonStream);
        
        bool TryGetWorld(const std::string& guid, GameWorld** worldOut);
    
        std::map<std::string, GameWorld*> loadedWorlds;
    };
}