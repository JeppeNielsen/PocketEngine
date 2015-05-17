//
//  FacebookUser.cpp
//  Facebook
//
//  Created by Jeppe Nielsen on 7/1/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "FacebookUser.hpp"
#include "minijson.h"

FacebookUser::FacebookUser() {}
FacebookUser::~FacebookUser() {}

void FacebookUser::UpdateFromJson(std::string json) {
    
    const char* jsonC = json.c_str();
    
    minijson::value v;
    
    minijson::error e = minijson::parse(jsonC, v);
    if (e == minijson::no_error) {
        
        minijson::object object = v.get<minijson::object>();
        
        ID = object["id"].get<std::string>();
        FirstName = object["first_name"].get<std::string>();
        LastName = object["last_name"].get<std::string>();
        FullName = object["name"].get<std::string>();
        Gender = object["gender"].get<std::string>() == "male" ? FacebookUser::Gender::Male : FacebookUser::Gender::Female;
        
        //std::cout<<"First name: " <<list["first_name"].get<std::string>()<<endl;
        //std::cout<<"Full name: " <<list["id"].get<std::string>()<<endl;
        
    }
}