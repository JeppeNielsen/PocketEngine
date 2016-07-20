//
//  FileHelper.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 20/07/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>

class FileHelper {
public:    
    static void FindFiles(std::vector<std::string>& list, const std::string& path, const std::string& extension);
    
private:
    static void FindFilesAtPath(std::vector<std::string>& list, const std::string& path, const std::string& extension);
};
