//
//  FieldInfoIndexer.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 10/02/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <string>
#include <vector>
//#include "Vector2.hpp"
//#include "Vector3.hpp"
#include "Property.hpp"

template<typename T> struct FieldInfoIndexer { static int Index() { return 0; } };

template<> struct FieldInfoIndexer<int> { static int Index() { return 0; } };
template<> struct FieldInfoIndexer<float> { static int Index() { return 1; } };
template<> struct FieldInfoIndexer<double> { static int Index() { return 2; } };
template<> struct FieldInfoIndexer<std::string> { static int Index() { return 3; } };
//template<> struct FieldInfoIndexer<Pocket::Vector2> { static int Index() { return 4; } };
//template<> struct FieldInfoIndexer<Pocket::Vector3> { static int Index() { return 5; } };

template<typename T> struct FieldInfoIndexer<std::vector<T>> { static int Index() { return 1000 + FieldInfoIndexer<T>::Index(); } };
template<typename T> struct FieldInfoIndexer<Pocket::Property<T>> { static int Index() { return 2000 + FieldInfoIndexer<T>::Index(); } };
