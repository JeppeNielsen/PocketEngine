//
//  ScriptClass.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 31/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <map>
#include <vector>

namespace Pocket {
  
class ScriptClass {
public:
    using Children = std::map<std::string, ScriptClass>;
    Children children;
    
    ScriptClass* parent;

    std::string name;
    std::string baseClass;
    std::string sourceFile;
    
    using TemplateArguments = std::vector<std::string>;
    TemplateArguments templateArguments;
    
    struct Field {
        std::string name;
        std::string type;
    };
    
    using Fields = std::vector<Field>;
    Fields fields;
    
    void Print();
};

}