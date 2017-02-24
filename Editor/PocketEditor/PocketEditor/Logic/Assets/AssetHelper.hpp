//
//  AssetHelper.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 24/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <functional>

struct AssetHelper {
    static bool CreateFolder(const std::string& path);
    static bool CreateScene(const std::string& path);
    static bool CreateEmptyPrefab(const std::string& path);
    static bool CreateHeader(const std::string& path);
    static bool CreateSource(const std::string& path);
    static bool CreateComponent(const std::string& path);
    static bool TryCreateAsset(const std::string& path, const std::string& message, const std::string& defaultName, const std::string& extension, std::function<void(const std::string& path)> onCreate);
    static bool CreateTextFile(const std::string& path, const std::string& content);
};
