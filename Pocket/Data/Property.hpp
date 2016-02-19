//
//  Property.h
//  EntitySystem
//
//  Created by Jeppe Nielsen on 08/12/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include "Event.hpp"

namespace Pocket {

template<class Value>
class Property {
private:
    Value value;
    
    void Set(const Value& newValue) {
        if (value==newValue) return;
        previousValue = value;
        value = newValue;
        Changed(value);
    }
    static Value previousValue;
public:
    Property() = default;

    Event<Value&> Changed;
    
    Value& PreviousValue() const;
    
    const Value& operator() () const { return value; }
    operator const Value& () const { return value; }
    
    void operator = (const Value& v) { Set(v); }
    void operator = (const Property<Value>& v) { Set(v.value); }
    Value operator - () const { return -value; }
    
    Value operator + (const Value& v) const { return value + v; }
    Value operator - (const Value& v) const { return value - v; }
    Value operator * (const Value& v) const { return value * v; }
    Value operator / (const Value& v) const { return value / v; }
    Value operator ++ (int) { Value temp = value; Set(value+1); return temp; }
    Value operator -- (int) { Value temp = value; Set(value-1); return temp; }
    Value operator ++ () { Set(value+1); return value; }
    Value operator -- () { Set(value-1); return value; }
    
    Value operator + (const Property<Value>& v) const { return value + v.value; }
    Value operator - (const Property<Value>& v) const { return value - v.value; }
    Value operator * (const Property<Value>& v) const { return value * v.value; }
    Value operator / (const Property<Value>& v) const { return value / v.value; }
    
    const Value& operator += (const Value& v) { Set(value + v); return value; }
    const Value& operator -= (const Value& v) { Set(value - v); return value; }
    const Value& operator *= (const Value& v) { Set(value * v); return value; }
    const Value& operator /= (const Value& v) { Set(value / v); return value; }

    const Value& operator += (const Property<Value&> v) { Set(value + v.value); return value; }
    const Value& operator -= (const Property<Value>& v) { Set(value - v.value); return value; }
    const Value& operator *= (const Property<Value>& v) { Set(value * v.value); return value; }
    const Value& operator /= (const Property<Value>& v) { Set(value / v.value); return value; }

    friend Value operator+(const Value& value, Property<Value>& property) {
        return value + property.value;
    }
    
    friend Value operator-(const Value& value, Property<Value>& property) {
        return value - property.value;
    }
    
    friend Value operator*(const Value& value, Property<Value>& property) {
        return value * property.value;
    }
    
    friend Value operator/(const Value& value, Property<Value>& property) {
        return value / property.value;
    }
    
    Property(const Property<Value>& other) {
        value = other.value;
    }
};

template<class Value>
Value Property<Value>::previousValue;

template<class Value>
Value& Property<Value>::PreviousValue() const {
    return previousValue;
}

};
