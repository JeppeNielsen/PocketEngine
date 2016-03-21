//
//  Components.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once

#include "TypeInfo.hpp"

using namespace Pocket;

struct Transform {
	int x;
	int y;
    
    //Transform() { std::cout<<"Transform ctor"<<std::endl; }
    //~Transform() { std::cout<<"Transform dtor"<<std::endl; }
    
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(x)
    TYPE_FIELD(y)
    TYPE_FIELDS_END
};

struct Velocity {
	int x;
	int y;
    //Velocity() { std::cout<<"Velocity ctor"<<std::endl; }
    //~Velocity() { std::cout<<"Velocity dtor"<<std::endl; }
    
    struct Info {
        int TestVar;
        
        TYPE_FIELDS_BEGIN
        TYPE_FIELD(TestVar)
        TYPE_FIELDS_END
    };
    
    Info info;
    
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(x)
    TYPE_FIELD(y)
    TYPE_FIELD(info)
    TYPE_FIELDS_END
};

struct Renderable {
	int imageNo;
    //Renderable() { std::cout<<"Renderable ctor"<<std::endl; }
    //~Renderable() { std::cout<<"Renderable dtor"<<std::endl; }
    
    //TYPE_FIELDS_BEGIN
    //TYPE_FIELD(imageNo)
    //TYPE_FIELDS_END
};

struct Touchable {
	bool clickThrough;
    //Touchable() { std::cout<<"Renderable ctor"<<std::endl; }
    //~Touchable() { std::cout<<"Renderable dtor"<<std::endl; }
    
    TYPE_FIELDS_BEGIN
    TYPE_FIELD(clickThrough)
    TYPE_FIELDS_END
};
