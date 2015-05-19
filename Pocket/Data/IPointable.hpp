//
//  IPointable.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 09/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "Pointer.hpp"

namespace Pocket {
    template<class T>
    class IPointable {
    public:
        IPointable();
        ~IPointable();
        IPointable(const IPointable<T>& other);
        IPointable& operator = (IPointable<T>& other);
        
        void ClearPointers();
        
    friend class Pointer<T>;

        typedef std::vector<Pointer<T>*> Pointers;
        Pointers pointers;
    };
}

template<class T>
Pocket::IPointable<T>::IPointable() {
    pointers.clear();
}

template<class T>
Pocket::IPointable<T>::IPointable(const IPointable<T>& other) {
    pointers.clear();
}

template<class T>
Pocket::IPointable<T>& Pocket::IPointable<T>::operator = (IPointable<T>& other) {
    pointers.clear();
    return *this;
}

template<class T>
Pocket::IPointable<T>::~IPointable() {
    ClearPointers();
}

template<class T>
void Pocket::IPointable<T>::ClearPointers() {
    if (!pointers.empty()) {
        int test = 0;
        test++;
    }
    for (size_t i=0; i<pointers.size(); ++i) {
        pointers[i]->object = 0;
    }
    pointers.clear();
}
