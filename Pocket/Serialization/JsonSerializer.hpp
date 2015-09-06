//
//  Stringifier.h
//  GUIEditor
//
//  Created by Jeppe Nielsen on 06/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "minijson_reader.hpp"
#include "minijson_writer.hpp"
#include "Property.hpp"
#include <type_traits>

namespace Pocket {

    template<class T, typename S = void>
    struct JsonSerializer {
        static void Serialize(std::string& key, const T& value, minijson::object_writer& writer) {
            std::stringstream s;
            s<<value;
            writer.write(key.c_str(), s.str());
        }
        
        static void Serialize(const T& value, minijson::array_writer& writer) {
            std::stringstream s;
            s<<value;
            writer.write(s.str());
        }
    };
    
    template<>
    struct JsonSerializer<int> {
        static void Serialize(std::string& key, const int& value, minijson::object_writer& writer) {
            writer.write(key.c_str(), value);
        }
        
        static void Serialize(const int& value, minijson::array_writer& writer) {
            writer.write(value);
        }
    };
    
    template<>
    struct JsonSerializer<float> {
        static void Serialize(std::string& key, const float value, minijson::object_writer& writer) {
            writer.write(key.c_str(), value);
        }
        
        static void Serialize(const float& value, minijson::array_writer& writer) {
            writer.write(value);
        }
    };
    
    template<>
    struct JsonSerializer<double> {
        static void Serialize(std::string& key, const double& value, minijson::object_writer& writer) {
            writer.write(key.c_str(), value);
        }
        
        static void Serialize(const double& value, minijson::array_writer& writer) {
            writer.write(value);
        }
    };
    
    template<>
    struct JsonSerializer<std::string> {
        static void Serialize(std::string& key, const std::string& value, minijson::object_writer& writer) {
            writer.write(key.c_str(), value);
        }
        
        static void Serialize(const std::string& value, minijson::array_writer& writer) {
            writer.write(value);
        }
    };
    
    template<typename I>
    struct JsonSerializer<std::vector<I>> {
        static void Serialize(std::string& key, const std::vector<I>& value, minijson::object_writer& writer) {
            minijson::array_writer array = writer.nested_array(key.c_str());
            for (int i=0; i<value.size(); ++i) {
                JsonSerializer<I>::Serialize(value[i], array);
            }
            array.close();
        }
        
        static void Serialize(const std::vector<I>& value, minijson::array_writer& writer) {
            minijson::array_writer array = writer.nested_array();
            for (int i=0; i<value.size(); ++i) {
                JsonSerializer<I>::Serialize(value[i], array);
            }
            array.close();
        }
    };
    
    template<typename Owner, typename T>
    struct JsonSerializer<Property<Owner, T>> {
        static void Serialize(std::string& key, const Property<Owner, T>& value, minijson::object_writer& writer) {
            JsonSerializer<T>::Serialize(key, value.GetValue(), writer);
        }
    };
}