#pragma once
#include "VectorHelper.hpp"

namespace Pocket {
	template<class T>
	class Timeline {
    public:
        
        using Keys = std::vector<float>;
        using Values = std::vector<T>;
        
        Keys keys;
        Values values;
        
        void AddNode(float time, const T& value) {
            keys.push_back(time);
            values.push_back(value);
            
            auto p = VectorHelper::SortPermutation(keys, [] (const float a, const float b) {
                return a<b;
            });
            
            keys = VectorHelper::ApplyPermutation(keys, p);
            values = VectorHelper::ApplyPermutation(values, p);
        }
        
        void Clear() {
            keys.clear();
            values.clear();
        }
        
        void RemoveNode(int index) {
            if (index<0 || index>=keys.size()) return;
            keys.erase(keys.begin() + index);
            values.erase(values.begin() + index);
        }
    
        int Count() { return (int)keys.size(); }
        
        template<typename Func>
        void Evaluate(float time, Func&& callback) {
            if (keys.empty()) return;
            if (time<keys[0]){
                callback(0, values[0], values[0]);
            } else if (time>keys.back()){
                callback(1.0f, values.back(), values.back());
            } else {
                for(int i=0; i<keys.size()-1; ++i) {
                    if (time<=keys[i+1]){
                        float interpolatedTime = (time - keys[i]) / (keys[i+1] - keys[i]);
                        callback(interpolatedTime, values[i], values[i+1]);
                        break;
                    }
                }
            }
        }
    };
}
