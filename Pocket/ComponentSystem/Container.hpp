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
        virtual int Create(int owner) = 0;
        virtual int CreateNoInit(int owner) = 0;
        virtual void Reference(int index) = 0;
        virtual void Delete(int index, int owner) = 0;
        virtual int Clone(int index, int owner) = 0;
        virtual void* Get(int index) = 0;
        virtual void Clear() = 0;
        virtual void Trim() = 0;
        virtual int GetOwner(int index) = 0;
        int Count() const { return count; }
        int count;
    };
    
    template<typename T>
    class Handle;
    
    template<typename T>
    using HandleCollection = std::vector<Handle<T>>;
    
    template<typename T>
    class Container : public IContainer {
    public:
        Container() : maxVersion(0) { }
        virtual ~Container() { }
    
        int Create(int owner) override {
            int freeIndex;
            if (freeIndicies.empty()) {
                freeIndex = (int)references.size();
                references.emplace_back(1);
                versions.emplace_back(maxVersion);
                entries.resize(freeIndex + 1);
                owners.emplace_back(owner);
            } else {
                freeIndex = freeIndicies.back();
                freeIndicies.pop_back();
                references[freeIndex] = 1;
                owners[freeIndex] = owner;
            }
            entries[freeIndex] = defaultObject;
            
            ++count;
            return freeIndex;
        }
        
        int CreateNoInit(int owner) override {
            int freeIndex;
            if (freeIndicies.empty()) {
                freeIndex = (int)references.size();
                references.emplace_back(1);
                versions.emplace_back(maxVersion);
                entries.resize(freeIndex + 1, defaultObject);
                owners.emplace_back(owner);
            } else {
                freeIndex = freeIndicies.back();
                freeIndicies.pop_back();
                references[freeIndex] = 1;
                owners[freeIndex] = owner;
            }
            
            ++count;
            return freeIndex;
        }

        void Reference(int index) override {
            ++references[index];
        }
        
        void Delete(int index, int owner) override {
            --references[index];
            if (references[index]==0) {
                ++versions[index];
                --count;
                freeIndicies.push_back(index);
                owners[index] = -1;
            } else if (owners[index] == owner) {
                owners[index] = -1;
            }
        }
        
        int Clone(int index, int owner) override {
            int cloneIndex = Create(owner);
            entries[cloneIndex] = entries[index];
            return cloneIndex;
        }
        
        void* Get(int index) override {
            return &entries[index];
        }
        
        void Clear() override {
            entries.clear();
            references.clear();
            versions.clear();
            count = 0;
        }
        
        void Trim() override {
            int smallestSize = 0;
            for(int i = (int)references.size() - 1; i>=0; --i) {
                if (references[i]>0) {
                    smallestSize = i + 1;
                    if (versions[i]>maxVersion) {
                        maxVersion = versions[i];
                    }
                    break;
                }
            }
            if (smallestSize<references.size()) {
                references.resize(smallestSize);
                versions.resize(smallestSize);
                entries.resize(smallestSize);
                for(int i=0; i<freeIndicies.size(); ++i) {
                    if (freeIndicies[i]>=smallestSize) {
                        freeIndicies.erase(freeIndicies.begin() + i);
                        --i;
                    }
                }
            }
        }
        
        int GetOwner(int index) override {
            return owners[index];
        }
        
        void Iterate(const std::function<void(T*)>& callback) {
            for(int i=0; i<references.size(); ++i) {
                if (references[i]>0) callback(&entries[i]);
            }
        }
        
        Handle<T> GetHandle(int index) const;
        
        HandleCollection<T> GetCollection() const {
            HandleCollection<T> collection;
            for(int i=0; i<references.size();++i) {
                if (references[i]>0) {
                    collection.emplace_back(Handle<T>(this, i, versions[i]));
                }
            }
            return collection;
        }
    
        using Entries = std::deque<T>;
        Entries entries;
        
        using References = std::vector<int>;
        References references;
        
        using Versions = std::vector<int>;
        Versions versions;
        
        using Owners = std::vector<int>;
        Owners owners;
        
        using FreeIndicies = std::vector<int>;
        FreeIndicies freeIndicies;
        
        int maxVersion;
        
        T defaultObject;
    };
    
    template<typename T>
    class Handle {
    private:
        const Container<T>* container;
        int index;
        int version;
        Handle(const Container<T>* container, int index, int version) :
            container(container), index(index), version(version) {
        }
    public:
        Handle() : container(0) {  }
        ~Handle() {}
        Handle(const Handle<T>& other) {
            container = other.container;
            index = other.index;
            version = other.index;
        }
        
        Handle(T* ptr) : Handle(ptr->GetHandle()) { }
        Handle(T& ptr) : Handle(ptr.GetHandle()) { }
        
        T* operator -> () const {
            if (!container) return 0;
            if (index>=container->versions.size()) return 0;
            if (version != container->versions[index]) return 0;
            return (T*)&container->entries[index];
        }
        
        explicit operator bool() const {
            return operator->();
        }
        
        friend class Container<T>;
    };
    
    template<typename T>
    Handle<T> Container<T>::GetHandle(int index) const {
        return Handle<T>(this, index, versions[index]);
    }
    
}