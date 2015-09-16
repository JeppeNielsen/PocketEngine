//
//  Event.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 6/14/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include "Delegate.hpp"

#ifdef ANDROID
#include <stdlib.h>
#endif

namespace Pocket {
   
template<class T>
class Event {
public:
    Event();
    ~Event();
    
    Event(const Event<T>& other);
    
    void operator = (const Event<T>& other);
    void operator ()(T value);
    void operator += (IDelegate<T>* delegate);
    void operator -= (IDelegate<T>* delegate);
    
    int Count();
    void Clear();
    bool Empty();
    
private:
    typedef std::vector<IDelegate<T>*> Delegates;
    Delegates delegates;
};

template<class T>
Event<T>::Event() {}

template<class T>
Event<T>::~Event() {
    for (size_t i=0; i<delegates.size(); i++) {
        delete delegates[i];
    }
}

template<class T>
Event<T>::Event(const Event<T>& other) { }

template<class T>
void Event<T>::operator=(const Event<T>& other) { }

template<class T>
void Event<T>::operator()(T value) {
    for (size_t i = 0; i<delegates.size(); i++) {
        delegates[i]->Invoke(value);
    }
}

template<class T>
void Event<T>::operator += (IDelegate<T>* delegate) {
    delegates.push_back(delegate);
}

template<class T>
void Event<T>::operator -= (IDelegate<T>* delegate) {
    for (size_t i = 0; i<delegates.size(); i++) {
        if (delegates[i]->Equals(delegate)) {
            delete delegates[i];
            delegates.erase(delegates.begin() + i);
            break;
        }
    }
    delete delegate;
}
    
template<class T>
int Event<T>::Count() { return (int)delegates.size(); }

template<class T>
void Event<T>::Clear() {
    size_t size = delegates.size();
    for (size_t i = 0; i<size; i++) {
        delete delegates[i];
    }
    delegates.clear();
}

template<class T>
bool Event<T>::Empty() { return delegates.empty(); }
    
template<class Target, class T>
IDelegate<T>* event_handler(Target* target, void (Target::* Method)(T value)) {
	return new Delegate<Target, T>(target, Method);
}

template<class Target, class Context, class T>
IDelegate<T>* event_handler(Target* target, void (Target::* Method)(T value, Context context), Context context) {
	return new DelegateContext<Target, Context, T>(target, Method, context);
}

}