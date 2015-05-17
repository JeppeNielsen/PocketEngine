#pragma once
#include <map>
#include <vector>

namespace Pocket {
	template<class T>
	class Timeline
	{
	public:

		typedef float Time;

		Timeline();
		~Timeline();

		void Add(Time time, const T& node);
		void Clear();
		
		void Get(Time time, T** source, T** dest, float& t);

		Time StartTime();
		Time EndTime();
		
		bool Empty();
        int Count();

    private:
        struct Node {
            Time time;
            T item;
        };
    
		typedef std::vector<Node> Nodes;
		Nodes nodes;
		Time startTime;
		Time endTime;
        
		bool isDirty;
		void CalcTime();
	};
}

template<class T>
Pocket::Timeline<T>::Timeline() {
	startTime = 0;
	endTime = 0;
	isDirty = false;
}

template<class T>
Pocket::Timeline<T>::~Timeline() {}

template<class T>
void Pocket::Timeline<T>::Add(Time time, const T& node) {
	nodes.push_back({ time, node });
	isDirty = true;
}

template<class T>
void Pocket::Timeline<T>::Clear() {
    nodes.clear();
    isDirty = true;
}

template<class T>
int Pocket::Timeline<T>::Count() {
    return (int)nodes.size();
}

template<class T>
bool Pocket::Timeline<T>::Empty() {
	return nodes.empty();
}

template<class T>
void Pocket::Timeline<T>::CalcTime() {
	if (!isDirty) return;
	isDirty = false;
	if (nodes.empty()) return;
    startTime = nodes[0].time;
    endTime = nodes[nodes.size()-1].time;
}

template<class T>
typename Pocket::Timeline<T>::Time Pocket::Timeline<T>::StartTime() {
	CalcTime();
	return startTime;
}

template<class T>
typename Pocket::Timeline<T>::Time Pocket::Timeline<T>::EndTime() {
	CalcTime();
	return endTime;
}

template<class T>
void Pocket::Timeline<T>::Get(Time time, T** source, T** dest, float& t) {
    t = 0;
    if (nodes.empty()) {
        source = 0;
        dest = 0;
        return;
    }
 	CalcTime();
	
	if (time<=startTime) {
		*source = &nodes[0].item;
        *dest = *source;
    } else if (time>=endTime) {
        *source = &nodes[nodes.size()-1].item;
        *dest = *source;
    } else {
        for (size_t i=nodes.size()-2; i>=0; --i) {
            if (time>=nodes[i].time) {
                *source = &nodes[i].item;
                *dest = &nodes[i+1].item;
                t = (time-nodes[i].time) / (nodes[i+1].time - nodes[i].time);
                return;
            }
        }
    }
}
