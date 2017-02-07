//
//  FileArchive.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 06/02/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <map>
#include <functional>

namespace Pocket {
    class FileArchive {
    public:
        static bool TryCreateArchiveFile(const std::string& path, const std::string& archiveFile, const std::function<std::string(const std::string& file)>& onFileParsed);
        
        FileArchive();
        ~FileArchive();
    
        bool Initialize(const std::string& path);
        
        bool TryLoadData(const std::string& id, const std::function<void(void*, size_t)>& onData);
        
        struct File {
            int index;
            std::string filename;
            size_t compressedSize;
            size_t uncompressedSize;
        };
        
        using Files = std::map<std::string, File>;
        const Files& GetFiles();
    private:
        std::string path;
        std::map<std::string, File> files;
        void* archive;
    };
}
