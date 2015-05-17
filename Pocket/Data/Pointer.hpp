//
//  Pointer.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 09/05/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>

namespace Pocket {
    template<class T>
    class IPointable;
    
    template<class T>
    class Pointer {
    public:
        Pointer();
        ~Pointer();
        Pointer(const Pointer<T>& other);
        Pointer<T>& operator = (Pointer<T>& other);
        Pointer<T>& operator = (Pointer<T> other);
        
        void operator = (T* object);
        T* operator -> ();
        T* operator () ();
        
        operator T* ();
  
        friend class IPointable<T>;
    private:
        void SetObject(T* newObject);
        int indexInList;
        T* object;
    };
}

template<class T>
Pocket::Pointer<T>::Pointer() : object(0) {}

template<class T>
Pocket::Pointer<T>::~Pointer() {
    SetObject(0);
}

template<class T>
Pocket::Pointer<T>::Pointer(const Pocket::Pointer<T>& other) {
    object = 0;
    SetObject(other.object);
}

template<class T>
Pocket::Pointer<T>& Pocket::Pointer<T>::operator=(Pointer<T> &other) {
    object = 0;
    SetObject(other.object);
    return *this;
}

template<class T>
Pocket::Pointer<T>& Pocket::Pointer<T>::operator=(Pointer<T> other) {
    object = 0;
    SetObject(other.object);
    return *this;
}

template<class T>
void Pocket::Pointer<T>::operator = (T* object) {
    SetObject(object);
}

template<class T>
T* Pocket::Pointer<T>::operator->() {
    return object;
}

template<class T>
T* Pocket::Pointer<T>::operator() () {
    return object;
}

template<class T>
Pocket::Pointer<T>::operator T* () {
    return object;
}

template<class T>
void Pocket::Pointer<T>::SetObject(T* newObject) {
    if (object) {
        object->pointers[indexInList] = object->pointers.back();
        object->pointers.pop_back();
    }
    object = newObject;
    if (object) {
        indexInList = (int)object->pointers.size();
        object->pointers.push_back(this);
    }
}
