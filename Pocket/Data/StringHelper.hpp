//
//  StringHelper.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/13/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include <vector>
#include <string>
#include <sstream>

namespace Pocket {
    class StringHelper {
    public:
        static std::vector<unsigned short> GetUTF(const std::string& s);
        
        static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
        static std::vector<std::string> split(const std::string &s, char delim);
        static void splitToFloats(const std::string& s, char delim, std::vector<float>& floats);
        static void splitToInts(const std::string& s, char delim, std::vector<int>& ints);
        
        //static std::string ConvertUTF8ToString(std::string utf8);
        static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
        static std::string base64_decode(const std::string& encoded_string);
        
        static std::string CreateGuid();
        
        static bool EndsWith(const std::string& str, const std::string& endsWith);
    };
}
