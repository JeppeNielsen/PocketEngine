//
//  TestJson.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 03/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "minijson_writer.hpp"
#include "minijson_reader.hpp"

using namespace minijson;

int main_write() {

    std::ofstream myfile;
    myfile.open ("example.txt");

    std::stringstream s;

    minijson::object_writer writer(myfile);
    writer.write("name", "Los Angeles");
    {
        minijson::object_writer position_writer = writer.nested_object("position");
        position_writer.write("n", 34.05);
        position_writer.write("w", 118.25);
        position_writer.close();
    }
    {
        minijson::array_writer mayors_writer = writer.nested_array("mayors");
        mayors_writer.write("Villaraigosa");
        mayors_writer.write("Garcetti");
        mayors_writer.close();
    }
    writer.close();
    myfile.close();

    std::cout<<s.str()<<std::endl;

    return 0;
}


int main() {

    std::ifstream myfile ("example.txt");

    minijson::istream_context ctx(myfile);

    minijson::parse_object(ctx, [&] (const char* name, minijson::value v) {
    
        std::cout<<name<<std::endl;
        if (v.type() == minijson::Object)
        {
            minijson::parse_object(ctx, [&] (const char* name, minijson::value v) {
                std::cout<< v.as_string()<< std::endl;
            });
        } else if ( v.type() == minijson::Array) {
            minijson::parse_array(ctx, [&] (minijson::value v) {
                std::cout<< v.as_string()<< std::endl;
            });
        }
    });

    myfile.close();
    return 0;
}







