#pragma once//;
#include "Event.hpp"

namespace Pocket {
	
	template<class Owner, class T> class DirtyProperty {

	public:
		DirtyProperty(Owner owner);
		~DirtyProperty();
        
        void operator = (DirtyProperty<Owner, T> &other);
        
		struct EventData {
			Owner owner;
			T* Value;
		};

		Event<EventData&> Method;

		void MakeDirty();
		Event<DirtyProperty<Owner, T>* > HasBecomeDirty; 

		T* GetValue();

		operator const T& ();

		Owner GetOwner();
		
	private:
		T value;
		bool isDirty;

		EventData eventData;

	};
}

template<class Owner, class T> Pocket::DirtyProperty<Owner, T>::DirtyProperty(Owner owner) { 
	isDirty = true; 
	eventData.owner = owner;
	eventData.Value = &value;
}

template<class Owner, class T> Pocket::DirtyProperty<Owner, T>::~DirtyProperty() { }

template<class Owner, class T>
void Pocket::DirtyProperty<Owner, T>::operator = (DirtyProperty<Owner, T> &other) {
	isDirty = other.isDirty;
    value = other.value;
}

template<class Owner, class T> void Pocket::DirtyProperty<Owner, T>::MakeDirty() {
	if (isDirty) return;
	isDirty = true;
	HasBecomeDirty(this);
}

template<class Owner, class T> T* Pocket::DirtyProperty<Owner, T>::GetValue() {
	if (isDirty) {
		isDirty = false;
		Method(eventData);
	}
	return &value;
}

template<class Owner, class T> Pocket::DirtyProperty<Owner, T>::operator const T& () {
	return *GetValue();
}

template<class Owner, class T> Owner Pocket::DirtyProperty<Owner, T>::GetOwner() {
	return eventData.owner;
}