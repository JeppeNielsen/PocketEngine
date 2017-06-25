//
//  IOSBuilder.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 25/06/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
#include "PlatformBuilder.hpp"

namespace Pocket {
    class IOSBuilder : public PlatformBuilder {
    public:
        
        void Build(const std::string& outputPath, const std::string& pocketEngineLibPath);
        
    private:
    
        void CreateXCodeProject(const std::string& outputFile);
        
        std::string GetXCodeProjectTemplate();
        std::string GetXCodePlistTemplate();
        std::string GetSplashPngData();
        std::string GetMainFile();
        
        friend class PlatformBuilder;
    };
}
