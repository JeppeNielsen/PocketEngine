//
//  Container.hpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 06/06/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <deque>
#include <vector>
#include <assert.h>
#include <functional>

namespace Pocket {

    class IContainer {
    public:
        virtual ~IContainer() {}
        virtual int Create() = 0;
        virtual void Reference(int index) = 0;
        virtual void Delete(int index) = 0;
        virtual int Clone(int index) = 0;
        virtual void* Get(int index) = 0;
        virtual void Clear() = 0;
        virtual void Trim() = 0;
        int Count() const { return count; }
        int count;
    };

    template<typename T>
    class Container : public IContainer {
    public:
        Container()  { count = 0; }
        virtual ~Container() { }
    
        int Create() override {
            int freeIndex;
            if (freeIndicies.empty()) {
                freeIndex = (int)references.size();
                references.emplace_back(0);
                entries.resize(freeIndex + 1);
            } else {
                freeIndex = freeIndicies.back();
                freeIndicies.pop_back();
            }
            
            ++count;
            entries[freeIndex] = defaultObject;
            references[freeIndex] = 1;
            return freeIndex;
        }
        
        void Reference(int index) override {
            ++references[index];
        }
        
        void Delete(int index) override {
            --references[index];
            if (references[index]==0) {
                --count;
                freeIndicies.push_back(index);
            }
        }
        
        int Clone(int index) override {
            int cloneIndex = Create();
            entries[cloneIndex] = entries[index];
            return cloneIndex;
        }
        
        void* Get(int index) override {
            return &entries[index];
        }
        
        void Clear() override {
            entries.clear();
            references.clear();
            count = 0;
        }
        
        void Trim() override {
            int smallestSize = 0;
            for(int i = (int)references.size() - 1; i>=0; --i) {
                if (references[i]>0) {
                    smallestSize = i + 1;
                    break;
                }
            }
            if (smallestSize<references.size()) {
                references.resize(smallestSize);
                entries.resize(smallestSize);
                for(int i=0; i<freeIndicies.size(); ++i) {
                    if (freeIndicies[i]>=smallestSize) {
                        freeIndicies.erase(freeIndicies.begin() + i);
                        --i;
                    }
                }
            }
        }
    
        using Entries = std::deque<T>;
        Entries entries;
        
        using References = std::vector<int>;
        References references;
        
        using FreeIndicies = std::vector<int>;
        FreeIndicies freeIndicies;
        
        T defaultObject;
    };
}