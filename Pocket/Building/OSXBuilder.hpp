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
    class OSXBuilder : public PlatformBuilder {
    public:
        
        void Build(const std::string& outputPath, const std::string& pocketEngineLibPath);
        
    private:
        
        void CreatePlistInfoFile(const std::string& output);
        void CreateIcon(const std::string& sourceImage, const std::string& destIcon);
        
        friend class PlatformBuilder;
    };
}
