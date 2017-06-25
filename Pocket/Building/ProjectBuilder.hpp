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

namespace Pocket {
    class ProjectBuilder {
    public:
        
        ProjectBuilder();
    
        void Initialize(const std::string& pocketEngineIncludePath, const std::string& workingDirectory);
        void SetSourceFiles(const std::vector<std::string>& sourceFiles,
                            const std::vector<std::string>& headerFiles
                           );
        
        IOSBuilder IOS;
        
    private:
        
        std::vector<std::string> GetEngineHeaders();
        
        std::vector<std::string> sourceFiles;
        std::vector<std::string> headerFiles;
        std::string pocketEngineIncludePath;
        std::string workingDirectory;
        
        friend class PlatformBuilder;
        friend class IOSBuilder;
    };
}
