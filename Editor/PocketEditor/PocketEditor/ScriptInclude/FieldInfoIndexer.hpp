//
//  FieldInfoIndexer.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>

template<typename T> struct FieldInfoIndexer { static int Index() { return 0; } };

template<> struct FieldInfoIndexer<int> { static int Index() { return 0; } };
template<> struct FieldInfoIndexer<float> { static int Index() { return 1; } };
template<> struct FieldInfoIndexer<double> { static int Index() { return 2; } };
template<> struct FieldInfoIndexer<std::string> { static int Index() { return 3; } };
