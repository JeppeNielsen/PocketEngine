//
//  Guid.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 15/10/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//
#pragma once
#include <array>
#include <iostream>
#include <string>

namespace Pocket {
    class Guid {
        public:
            Guid();
            Guid(const Guid& other);
            Guid(Guid&& other);
        
            bool operator == (const Guid& other);
            bool operator != (const Guid& other);
            void operator = (const Guid& other);
        
            static Guid CreateNew();
        
            std::string ToString() const;
            void FromString(const std::string& s);
        private:
            std::array<unsigned char, 16> bytes;
        public:
            inline friend std::ostream& operator<<(std::ostream& stream, const Guid& g) {
                stream<<g.ToString();
                return stream;
            }
            Guid static Deserialize(const std::string& data);
    };
}