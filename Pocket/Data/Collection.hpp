#pragma once

#include <algorithm>
#include <vector>
#include "Event.hpp"

namespace Pocket {

	template<class T> class Collection {
	public:
		Collection();
		~Collection();

		void Add(T object);
		void Remove(T object);
        void AddFirst(T object);
		
		typedef std::vector<T> List;

		Pocket::Event<T> Added;
		Pocket::Event<T> Removed;

		int Count();

		T Get(int index);
        
        typedef bool (SortMethod)(T a, T b);
        
        void Sort(SortMethod sortMethod);
        
        //bool Greater(const TRect &a, const TRect &b)

	private:
		List list;

	};
}

template<class T> Pocket::Collection<T>::Collection() { }
template<class T> Pocket::Collection<T>::~Collection() { }

template<class T> void Pocket::Collection<T>::Add(T object) {
	list.push_back(object);
	Added(object);
}

template<class T> void Pocket::Collection<T>::Remove(T object) {
	//List::iterator it = std::find(list.begin(), list.end(), object);
	//if (it==list.end()) return;
	list.erase(std::find(list.begin(), list.end(), object));
	Removed(object);
}

template<class T> void Pocket::Collection<T>::AddFirst(T object) {
	list.insert(list.begin(), object);
	Added(object);
}

template<class T> int Pocket::Collection<T>::Count(){
	return list.size();
}

template<class T> T Pocket::Collection<T>::Get(int index) {
	return list[index];
}

template<class T> void Pocket::Collection<T>::Sort(SortMethod sortMethod) {
    std::sort(list.begin(), list.end(), sortMethod);
}