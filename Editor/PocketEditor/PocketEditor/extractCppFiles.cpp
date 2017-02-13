//
//  extractCppFiles.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 08/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "FileHelper.hpp"
#include <iostream>
#include <set>
#include <vector>
#include "StringHelper.hpp"
#include <algorithm>

using namespace Pocket;

int main_extract() {

    std::string path = "/Projects/PocketEngine/Pocket";
    std::string preIncludePath = "-I $POCKET_PATH";
    std::string postIncludePath = "/ \\";
    
    std::string preSourcePath = "$POCKET_PATH";
    std::string postSourcePath = " \\";
    
    std::vector<std::string> ignoreFolders = {
        "Platform/Android",
        "Platform/OSX",
        "Platform/Windows",
        "Platform/iOS",
        
        "Logic/Animation/Spine",
        "Logic/Physics",
        "Logic/Audio",
        
        "Physics/Box2D",
        "Physics/Bullet",
        
        "Other/Chromecast",
        "Other/Clipping",
        
        "Threads",
        
        "Scripting"
    };
    
    std::vector<std::string> ignoreSourceFiles = {
        "DeferredRenderSystem.cpp",
        "DeferredBuffers.cpp",
        "Terrain.cpp",
        "TerrainSystem.cpp"
    };

    std::set<std::string> headerFolders;
    std::vector<std::string> cppFiles;
    std::vector<std::string> cFiles;
    

    FileHelper::RecurseFolder(path, [&] (const std::string& path) {
    
        std::string folder = FileHelper::GetFolderPath(path);
        for(auto ignore : ignoreFolders) {
            auto n = folder.find(ignore);
            if (n != std::string::npos) return;
        }
    
        if (StringHelper::EndsWith(path, ".hpp") || StringHelper::EndsWith(path, ".h")) {
            headerFolders.insert(folder);
        } else {
            
            for(auto ignore : ignoreSourceFiles) {
                auto n = path.find(ignore);
                if (n != std::string::npos) return;
            }
            
            if (StringHelper::EndsWith(path, ".cpp")) {
                cppFiles.push_back(path);
            }
            else if (StringHelper::EndsWith(path, ".c")) {
                cFiles.push_back(path);
            }
        }
    });
    
    //std::sort(headerFolders.begin(), headerFolders.end());
    std::sort(cppFiles.begin(), cppFiles.end());
    std::sort(cFiles.begin(), cFiles.end());
    
    size_t pathLength = path.size();
    
    std::vector<std::string> relativeHeaderFolders;
    for(auto s : headerFolders) {
        relativeHeaderFolders.push_back(preIncludePath + s.substr(pathLength, s.size() - pathLength) + postIncludePath);
    }
    for(auto s : relativeHeaderFolders) {
        std::cout << s << std::endl;
    }
    
    std::vector<std::string> relativeCppFiles;
    for(auto s : cppFiles) {
        relativeCppFiles.push_back(preSourcePath + s.substr(pathLength, s.size() - pathLength) + postSourcePath);
    }
    
    std::vector<std::string> relativeCFiles;
    for(auto s : cFiles) {
        relativeCFiles.push_back(preSourcePath + s.substr(pathLength, s.size() - pathLength) + postSourcePath);
    }
    
    for(auto s : relativeCppFiles) {
        std::cout << s << std::endl;
    }
    for(auto s : relativeCFiles) {
        std::cout << s << std::endl;
    }
    
    /*
    std::string objectPath = "/Projects/PocketEngine/Projects/PocketEngine/Build/Build/Intermediates/PocketEngine.build/Debug/PocketEngine.build/Objects-normal/x86_64/";
    
    
    for(auto s : cppFiles) {
        std::string p = FileHelper::GetFileNameFromPath(s);
        
        std::cout << objectPath<< p.substr(0, p.size() - 4) + ".o" << std::endl;
    }
    for(auto s : cFiles) {
        std::string p = FileHelper::GetFileNameFromPath(s);
        
        std::cout <<objectPath<< p.substr(0, p.size() - 4) + ".o" << std::endl;
    }
    */
    
    
    
    
    return 0;
}
