#pragma once
#include "Event.hpp"

namespace Pocket {
	template<typename Value>
    class DirtyProperty {
	private:
        Value value;
        bool isDirty;
        
        Value& getValue() {
            if (isDirty) {
                isDirty = false;
                if (Method) {
                    Method(value);
                }
            }
            return value;
        }
    
    public:
        DirtyProperty() : isDirty(true) {}
        DirtyProperty(const DirtyProperty<Value>& other) {
            isDirty = true;
            value = other.value;
        }
    
	    void operator = (DirtyProperty<Value> &other) {
            isDirty = true;
            value = other.value;
        }
        
        std::function<void(Value&)> Method;
    
		void MakeDirty() {
            if (isDirty) return;
            isDirty = true;
            HasBecomeDirty();
        }
        
		Event<> HasBecomeDirty;

        const Value& operator() () { return getValue(); }
        operator const Value& () { return getValue(); }
        
        Value operator - () const { return -getValue(); }
        
        Value operator + (const Value& v) const { return getValue() + v; }
        Value operator - (const Value& v) const { return getValue() - v; }
        Value operator * (const Value& v) const { return getValue() * v; }
        Value operator / (const Value& v) const { return getValue() / v; }
        
        Value operator + (const DirtyProperty<Value>& v) const { return getValue() + v.getValue(); }
        Value operator - (const DirtyProperty<Value>& v) const { return getValue() - v.getValue(); }
        Value operator * (const DirtyProperty<Value>& v) const { return getValue() * v.getValue(); }
        Value operator / (const DirtyProperty<Value>& v) const { return getValue() / v.getValue(); }
        
        friend Value operator+(const Value& value, DirtyProperty<Value>& property) {
            return value + property.getValue();
        }
        
        friend Value operator-(const Value& value, DirtyProperty<Value>& property) {
            return value - property.getValue();
        }
        
        friend Value operator*(const Value& value, DirtyProperty<Value>& property) {
            return value * property.getValue();
        }
        
        friend Value operator/(const Value& value, DirtyProperty<Value>& property) {
            return value / property.getValue();
        }

	};
}