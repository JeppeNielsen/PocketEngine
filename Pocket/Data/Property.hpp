#pragma once
#include "Event.hpp"

#ifdef ANDROID
#include <ostream>
#endif

namespace Pocket {
	template<class Owner, class T> class Property {
	public:
		Property(Owner owner);
		~Property();
        
        Property(const Property<Owner, T> &other);

		void operator = (Property<Owner, T> &other);

		T Get() const;
		T operator () (void) const;
		T operator = (const T& other);

		operator const T () const;

		const T& GetValue() const;

		void Set(T value);
		Event<Owner> Changed;



		struct EventData {
			Owner owner;
			T Current;
			T Old;
		};

		Event<EventData> ChangedWithOld;

		T operator + (T value) const;
		T operator - (T value) const;
		T operator * (T value) const;
		T operator / (T value) const;
		T operator += (T value);
		T operator -= (T value);
		T operator *= (T value);
		T operator /= (T value);

	private:
		T value;
		Owner owner;

	};
}

template<class Owner, class T> Pocket::Property<Owner, T>::Property(Owner owner) {
	this->owner = owner;
}

template<class Owner, class T>
Pocket::Property<Owner, T>::Property(const Property<Owner, T> &other) {
	Set( other.Get() );
    Changed = other.Changed;
    ChangedWithOld = other.ChangedWithOld;
}

template<class Owner, class T> void Pocket::Property<Owner, T>::operator = (Property<Owner, T> &other) {
	Set( other.Get() );
}

template<class Owner, class T> Pocket::Property<Owner, T>::~Property() {
}

template<class Owner, class T> void Pocket::Property<Owner, T>::Set(T value) {
	if (this->value!=value) {
		if (ChangedWithOld.Count()>0) {
			EventData e;
			e.owner = owner;
			e.Current = value;
			e.Old = this->value;
			this->value = value;
			Changed(owner);
			ChangedWithOld(e);
		} else {
			this->value = value;
			Changed(owner);
		}
	}
}

template<class Owner, class T> T Pocket::Property<Owner, T>::Get() const {
	return value;
}

template<class Owner, class T> const T& Pocket::Property<Owner, T>::GetValue() const {
	return value;
}

template<class Owner, class T> T Pocket::Property<Owner, T>::operator () () const {
	return Get();
}

template<class Owner, class T> T Pocket::Property<Owner, T>::operator = (const T& other) {
	Set(other);
	return Get();
}

template<class Owner, class T> Pocket::Property<Owner, T>::operator const T () const {
	return Get();
}

template<class Owner, class T> T Pocket::Property<Owner, T>::operator + (T value) const {
	return this->value + value;
}

template<class Owner, class T> T Pocket::Property<Owner, T>::operator - (T value) const {
	return this->value - value;
}

template<class Owner, class T> T Pocket::Property<Owner, T>::operator * (T value) const {
	return this->value * value;
}

template<class Owner, class T> T Pocket::Property<Owner, T>::operator / (T value) const {
	return this->value / value;
}

template<class Owner, class T> T Pocket::Property<Owner, T>::operator += (T value) {
	Set(this->value + value);
	return Get();
}

template<class Owner, class T> T Pocket::Property<Owner, T>::operator -= (T value) {
	Set(this->value - value);
	return Get();
}

template<class Owner, class T> T Pocket::Property<Owner, T>::operator *= (T value) {
	Set(this->value * value);
	return Get();
}

template<class Owner, class T> T Pocket::Property<Owner, T>::operator /= (T value) {
	Set(this->value / value);
	return Get();
}

template<class Owner, class T>
std::ostream& operator<<(std::ostream& stream, const Pocket::Property<Owner, T>& v) {
    stream<<v();
    return stream;
}
