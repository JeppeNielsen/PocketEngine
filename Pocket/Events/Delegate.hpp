//
//  Delegate.hpp
//  ComponentSystem
//
//  Created by Jeppe Nielsen on 6/14/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#pragma once

namespace Pocket {
   
template<class T>
class Event;

template<class T>
class IDelegate {
protected:
    IDelegate();
    virtual ~IDelegate();
    
    virtual void Invoke(T value) = 0;
    virtual IDelegate<T>* Clone() = 0;
    virtual bool Equals(IDelegate<T>* other) = 0;
public:
    virtual int Type() = 0;

    friend class Event<T>;
};

template<class T>
IDelegate<T>::IDelegate() {}

template<class T>
IDelegate<T>::~IDelegate() {}

template<class Target, class T>
class Delegate : public IDelegate<T> {
public:
    typedef void (Target::* Method)(T value);
    
    Delegate(Target* target, Method method);
    virtual ~Delegate();
protected:
    void Invoke(T value);
    IDelegate<T>* Clone();
    bool Equals(IDelegate<T>* other);
    int Type();
private:

    Target* target;
    Method method;
    
    friend class Event<T>;
};

template<class Target, class T>
Delegate<Target, T>::Delegate(Target* target, Method method)
: target(target), method(method) {}

template<class Target, class T>
Delegate<Target, T>::~Delegate() {}

template<class Target, class T>
void Delegate<Target, T>::Invoke(T value) {
    (target->*method)(value);
}

template<class Target, class T>
IDelegate<T>* Delegate<Target, T>::Clone() {
    return new Delegate<Target, T>(target, method);
}

template<class Target, class T>
int Delegate<Target, T>::Type() {
    return 0;
}

template<class Target, class T>
bool Delegate<Target, T>::Equals(IDelegate<T> *other) {
    if (other->Type()!=0) return false;
    Delegate<Target, T>* delegate = (Delegate<Target, T>*)other;
    return target == delegate->target && method == delegate->method;
}


template<class Target, class Context, class T>
class DelegateContext : public IDelegate<T> {
public:
    typedef void (Target::* Method)(T value, Context context);
    
    DelegateContext(Target* target, Method method, Context context);
    virtual ~DelegateContext();
protected:
    void Invoke(T value);
    IDelegate<T>* Clone();
    int Type();
    bool Equals(IDelegate<T>* other);
private:
    
    Target* target;
    Method method;
    Context context;
    
    friend class Event<T>;
};

template<class Target, class Context, class T>
DelegateContext<Target, Context, T>::DelegateContext(Target* target, Method method, Context context)
: target(target), method(method), context(context) {}

template<class Target, class Context, class T>
DelegateContext<Target, Context, T>::~DelegateContext() {}

template<class Target, class Context, class T>
void DelegateContext<Target, Context, T>::Invoke(T value) {
    (target->*method)(value, context);
}

template<class Target, class Context, class T>
IDelegate<T>* DelegateContext<Target, Context, T>::Clone() {
    return new DelegateContext<Target, Context, T>(target, method, context);
}

template<class Target, class Context, class T>
int DelegateContext<Target, Context, T>::Type() {
    return 1;
}

template<class Target, class Context, class T>
bool DelegateContext<Target, Context, T>::Equals(IDelegate<T>* other) {
    if (other->Type()!=1) return false;
    DelegateContext<Target, Context, T>* delegate = (DelegateContext<Target, Context, T>*)other;
    return target == delegate->target && method == delegate->method && context == delegate->context;
}

}

