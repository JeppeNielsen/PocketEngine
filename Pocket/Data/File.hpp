//
//  File.h
//  Android
//
//  Created by Jeppe Nielsen on 15/07/15.
//
//

#pragma once
#include <string>

namespace Pocket {
    class File {
    public:
        File();
        File(std::string path);
        ~File();
        bool Load(std::string path);
        size_t Size() { return size; }
        unsigned char* Data() { return data; }
        std::string GetDataAsText() {
            if (!data || !size) return "";
            std::string text(reinterpret_cast<char*>(data), size);
            return text;
        }
        static std::string GetFullPath(std::string filename);
    private:
        size_t size;
        unsigned char* data;
        void* userData;
    };
}