//
//  NodeLine.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 4/20/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <algorithm>
#include <cmath>
#include "Event.hpp"

namespace Pocket {
    template<typename T>
    class NodeLine {
    public:
        NodeLine();
        ~NodeLine();
        
        NodeLine(const T& value);
        
        void Initialize(int gridSize, int countBeforeGrid);
        
        void Add(T item, float position);
        void Remove(size_t index);
        
        void Clear();
        
        float GetMinPosition();
        float GetMaxPosition();
        
        struct Result {
            T Source;
            T Dest;
            float t;
        };
        
        const Result& Get(float position);
        
        struct Node {
            float position;
            Node* prev;
            float size;
            T item;
        };
        
        typedef std::vector<Node*> NodesList;
        
        const NodesList& Nodes();
        
        size_t Count() const;
        
        void MakeDirty();
        
        Event<NodeLine*> HasBecomeDirty;
        
        bool Extrapolate;
        
    private:
        
        static bool Sort(const Node* a, const Node* b);
        
        NodesList nodes;
        
        typedef std::vector<NodesList> Grid;
        Grid grid;
        
        int gridSize;
        int countBeforeGrid;
        
        float minPosition;
        float maxPosition;
        float deltaPosition;
        size_t nodesCount;
        
        bool isDirty;
        
        void Calculate();
        
        Result result;
    };
}

template <typename T> Pocket::NodeLine<T>::NodeLine() { isDirty = false; Initialize(10, 5); Extrapolate = false; nodes.clear(); nodesCount = 0; }
template <typename T> Pocket::NodeLine<T>::~NodeLine() { Clear(); }
template <typename T> Pocket::NodeLine<T>::NodeLine(const T& value) { isDirty = false; Initialize(10, 5); Add(value, 0); }

template <typename T>
void Pocket::NodeLine<T>::Initialize(int gridSize, int countBeforeGrid) {
    this->gridSize = gridSize;
    this->countBeforeGrid = countBeforeGrid;
}

template <typename T>
void Pocket::NodeLine<T>::Add(T item, float position) {
    Node* node = new Node();
    node->position = position;
    node->item = item;
    nodes.push_back(node);
    MakeDirty();
}

template <typename T>
void Pocket::NodeLine<T>::Remove(size_t index) {
    if (index>=nodes.size()) return;
    nodes.erase(nodes.begin() + index);
    MakeDirty();
}

template <typename T>
const typename Pocket::NodeLine<T>::NodesList& Pocket::NodeLine<T>::Nodes() {
    MakeDirty();
    return nodes;
}

template <typename T>
void Pocket::NodeLine<T>::Clear() {
    for (typename NodesList::iterator it = nodes.begin(); it!=nodes.end(); ++it) {
        delete *it;
    }
    nodes.clear();
    MakeDirty();
}

template <typename T>
bool Pocket::NodeLine<T>::Sort(const typename Pocket::NodeLine<T>::Node* a, const typename Pocket::NodeLine<T>::Node* b) {
    return a->position<b->position;
}

template <typename T>
void Pocket::NodeLine<T>::Calculate() {
    if (!isDirty) return;
    isDirty = false;
    nodesCount = nodes.size();
    if (nodes.empty()) return;
        
    std::sort(nodes.begin(), nodes.end(), Pocket::NodeLine<T>::Sort);
    
    
    if (nodesCount==0) return;
    
    nodes[0]->size = 0;
    nodes[0]->prev = nodes[0];
    
    nodes[nodesCount - 1]->prev = nodes[nodesCount>1 ? nodesCount - 2 : 0];
    
    for (unsigned i=1; i<nodesCount; i++) {
        nodes[i]->prev = nodes[i-1];
        nodes[i]->size = nodes[i]->position - nodes[i]->prev->position;
    }
    
    minPosition = nodes[0]->position;
    maxPosition = nodes[nodesCount-1]->position;
    
    if (nodesCount<countBeforeGrid) return;
    
    
    deltaPosition = (gridSize - 1) / (maxPosition - minPosition);
    
    grid.clear();
    grid.resize(gridSize);
    
    for (unsigned i=0; i<nodesCount; i++) {
        Node* node = nodes[i];
        int startIndex = (int)floorf(((node->position - node->size) - minPosition) * deltaPosition);
        int endIndex = (int)floorf((node->position - minPosition) * deltaPosition);
        for (int o=startIndex; o<=endIndex; o++) {
            grid[o].push_back(node);
        }
    }
}

template <typename T>
const typename Pocket::NodeLine<T>::Result& Pocket::NodeLine<T>::Get(float position) {
    Calculate();
    
    if (position<=minPosition) {
        result.Source = nodes[0]->item;
        result.Dest = result.Source;
        result.t = 0;
        return result;
    } else if (position>=maxPosition) {
        if (!Extrapolate || nodesCount==1) {
            Node* node = nodes[nodesCount - 1];
            result.Source = node->item;
            result.Dest = result.Source;
            result.t = 1.0f;
        } else {
            Node* sourceNode = nodes[nodesCount - 2];
            Node* destNode = nodes[nodesCount - 1];
            result.Source = sourceNode->item;
            result.Dest = destNode->item;
            result.t = ((position - sourceNode->position) / destNode->size);
        }
        return result;
    }
    
    NodesList* list;
    
    if (nodesCount<countBeforeGrid) {
        list = &nodes;
    } else {
        int gridIndex = (int)floorf((position - minPosition) * deltaPosition);
        list = &grid[gridIndex];
    }
    
    for(unsigned i=0; i<list->size(); i++) {
        Node* node = list->at(i);
        if (node->position>position) {
            result.Source = node->prev->item;
            result.Dest = node->item;
            result.t = (position - node->prev->position) / node->size;
            return result;
        }
    }
    
    return result;
}

template <typename T>
float Pocket::NodeLine<T>::GetMinPosition() { Calculate(); return minPosition; }

template <typename T>
float Pocket::NodeLine<T>::GetMaxPosition() { Calculate(); return maxPosition; }

template <typename T>
size_t Pocket::NodeLine<T>::Count() const { return nodes.size(); }

template <typename T>
void Pocket::NodeLine<T>::MakeDirty() {
    isDirty = true;
    HasBecomeDirty(this);
}
