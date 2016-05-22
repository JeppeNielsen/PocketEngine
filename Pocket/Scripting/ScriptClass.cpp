//
//  ScriptClass.cpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 31/01/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "ScriptClass.hpp"
#include <iostream>

using namespace Pocket;

void ScriptClass::Print() {
    
    std::cout << "name : "<< name << std::endl;
    std::cout << "sourceFile : "<< sourceFile << std::endl;
    
    std::cout << "baseClass : "<< baseClass << std::endl;
    for(auto ta : templateArguments) {
        std::cout << "templateArgument : "<< ta << std::endl;
    }
    
    
    std::cout<<"Fields:"<<std::endl;
    for(auto& f : fields) {
        std::cout << f.type<<" "<< f.name << std::endl;
    }
    
    std::cout<<""<<std::endl;
    
    
    for(auto child : children) {
        child.second.Print();
    }
}

bool ScriptClass::HasFieldName(const std::string &name) {
    for(auto& f: fields) {
        if (f.name == name) return true;
    }
    return false;
}

