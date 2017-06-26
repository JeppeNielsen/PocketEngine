//
//  ProjectBuilder.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 24/06/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include "IOSBuilder.hpp"
#include "ScriptWorld.hpp"

namespace Pocket {
    class ProjectBuilder {
    public:
        
        ProjectBuilder();
    
        void Initialize(const std::string& pocketEngineIncludePath, const std::string& workingDirectory, const std::string& resourcesPath, ScriptWorld& world);
        void SetSourceFiles(const std::vector<std::string>& sourceFiles,
                            const std::vector<std::string>& headerFiles
                           );
        void SetStartupScene(const std::string& sceneGUID);
        
        IOSBuilder IOS;
        
    protected:
        bool CreateResources(const std::string& outputFile);
        
    private:
        
        std::vector<std::string> GetEngineHeaders();
        
        std::vector<std::string> sourceFiles;
        std::vector<std::string> headerFiles;
        std::string pocketEngineIncludePath;
        std::string workingDirectory;
        std::string resourcesPath;
        std::string startupSceneGUID;
        ScriptWorld* world;
        
        friend class PlatformBuilder;
        friend class IOSBuilder;
    };
}
