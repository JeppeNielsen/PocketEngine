//
//  FacebookUser.hpp
//  Facebook
//
//  Created by Jeppe Nielsen on 7/1/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>


class FacebookUser {
public:
    FacebookUser();
    ~FacebookUser();
    
    std::string ID;
    std::string FirstName;
    std::string LastName;
    std::string FullName;
    enum class Gender { Male, Female };
    Gender Gender;
    
    void UpdateFromJson(std::string json);
};