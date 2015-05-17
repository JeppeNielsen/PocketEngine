#pragma once
#include <vector>
#include "Box.hpp"
#include "ICellItem.hpp"

namespace Pocket {

	template<class T>
	class CellGrid {
	public:

		
		CellGrid();
		~CellGrid();

		bool StartIteration(const Box& box);
		bool GetNext(T& item);
		
		void Clear();
		void Add(T item, Box& size);
		void Remove(T item, Box& size);
		

		void SetSize(Box size);
		
	private:
		const static int gridSize = 32;
		const static int gridSizeIndex = gridSize - 1;
		
#define Limit(X) if (X<0) X = 0; else if (X>gridSizeIndex) X = gridSizeIndex;

		Box size;
		int cells;
		vector<T> lists[gridSize][gridSize];
		float multiplyX;
        float multiplyY;
		vector<T>* iterator[gridSize*gridSize];

		int iteratorCount;
		int iteratorIndex;
		int listIndex;

		static int counter;

	};
}

template<class T> Pocket::CellGrid<T>::CellGrid() {
	iteratorCount=0;
	iteratorIndex=0;
	listIndex=0;
}

template<class T> Pocket::CellGrid<T>::~CellGrid() {
}

template<class T> int Pocket::CellGrid<T>::counter = 0;

template<class T> void Pocket::CellGrid<T>::SetSize(Box size) {
	this->size = size;
	multiplyX = (float)gridSize / (this->size.right - this->size.left);
	multiplyY = (float)gridSize / (this->size.bottom - this->size.top);
	Clear();
}

template<class T> bool Pocket::CellGrid<T>::StartIteration(const Box& box) {
	if (!box.Intersect(size)) return false;

	int minX = (int)((box.left - size.left) * multiplyX);
	int minY = (int)((box.top - size.top) * multiplyY);

	int maxX = (int)((box.right - size.left) * multiplyX);
	int maxY = (int)((box.bottom - size.top) * multiplyY);

	Limit(minX);
	Limit(minY);
	Limit(maxX);
	Limit(maxY);

	iteratorCount = 0;
	
	for (int y=minY; y<=maxY; y++)
	{
		for (int x = minX; x<=maxX; x++) {
			iterator[iteratorCount++] = &lists[x][y];
		}
	}

	counter++;
	
	if (iteratorCount>0) {
		iteratorIndex =	0;
		listIndex = 0;
		return true;
	}
	return false;
}

template<class T> bool Pocket::CellGrid<T>::GetNext(T& item) {

	while (true) {

		if (iteratorIndex>iteratorCount - 1) return false;

		vector<T>* list = iterator[iteratorIndex];

		listIndex++;

		if (listIndex>list->size()) {
			iteratorIndex++;
			listIndex = 0;
			continue;
		}

		item = list->at(listIndex - 1);

		ICellItem* cellItem = (ICellItem*)item;
		
		if (cellItem->Index!=counter) {
			cellItem->Index = counter;
			return true;
		}
	}

	/*
	while (true) {
		vector<T>* list = iterator[iteratorIndex];
		if (listIndex<list->size()) {
			return list->at(listIndex++);
		} else {
			iteratorIndex++;
			if (iteratorIndex>=iteratorCount) {
				return false;
			}
			listIndex = 0;
		}
	}
	*/
	/*
	if (listIndex<list->size()) {
		item = listIter;
		return true;
	} else {
		listIndex = 0;
		iteratorIndex++;
		if (iteratorIndex<iteratorCount) {
			item = list[listIndex++];
			return true;
		} else {
			return false;
		}
	}
	*/
}

template<class T> void Pocket::CellGrid<T>::Clear() {
	for (int y=0; y<gridSize; y++) {
		for (int x = 0; x<gridSize; x++) {
			lists[x][y].clear();
		}
	}
}

template<class T> void Pocket::CellGrid<T>::Add(T item, Box& box) {
	int minX = (int)((box.left - size.left) * multiplyX);
	int minY = (int)((box.top - size.top) * multiplyY);

	int maxX = (int)((box.right - size.left) * multiplyX);
	int maxY = (int)((box.bottom - size.top) * multiplyY);

	Limit(minX);
	Limit(minY);
	Limit(maxX);
	Limit(maxY);

	for (int y=minY; y<=maxY; y++) {
		for (int x = minX; x<=maxX; x++) {
			lists[x][y].push_back(item);
		}
	}
}

template<class T> void Pocket::CellGrid<T>::Remove(T item, Box& box) {
	int minX = (int)((box.left - size.left) * multiplyX);
	int minY = (int)((box.top - size.top) * multiplyY);

	int maxX = (int)((box.right - size.left) * multiplyX);
	int maxY = (int)((box.bottom - size.top) * multiplyY);

	Limit(minX);
	Limit(minY);
	Limit(maxX);
	Limit(maxY);

	for (int y=minY; y<=maxY; y++) {
		for (int x = minX; x<=maxX; x++) {
			vector<T>& list = lists[x][y];
			typename vector<T>::iterator it = find(list.begin(), list.end(), item);
			if (it!=list.end()) {
				list.erase(it);
			}
		}
	}
}