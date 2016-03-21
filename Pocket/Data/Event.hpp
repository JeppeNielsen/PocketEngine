//
//  Event.hpp
//  TestEvents
//
//  Created by Jeppe Nielsen on 15/03/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <functional>

namespace Pocket {
template<typename... T>
class Event {
private:
    
    template<typename...Args>
    struct IDelegate {
        int type;
        virtual ~IDelegate() {}
        virtual void Invoke(Args... values) = 0;
    };
    using Delegate = IDelegate<T...>;
    using Delegates = std::vector<Delegate*>;
    Delegates delegates;
    
    template<typename Obj>
    struct IDelegateMember : public IDelegate<T...> {
        Obj* object;
        void (Obj::*method)(T...);
        
        void Invoke(T... values) override {
            (object->*method)(std::forward<T>(values)...);
        }
    };
    
    template<typename Obj, typename Context>
    struct IDelegateMemberContext : public IDelegate<T...> {
        Obj* object;
        Context context;
        void (Obj::*method)(T..., Context);
        
        void Invoke(T... values) override {
            (object->*method)(std::forward<T>(values)..., context);
        }
    };
    
    struct IDelegateLambda : public IDelegate<T...> {
        std::function<void(T...)> function;
        void Invoke(T... values) override {
            function(std::forward<T>(values)...);
        }
    };
    
    static int objectIDCounter;
    
    template<typename O>
    int GetObjectID() {
        static int id = objectIDCounter++;
        return id;
    }
    
public:
    
    Event() = default;
    Event(const Event<T...>& other) { delegates.clear(); }
    Event(Event<T...>& other) { delegates.clear(); }
    void operator=(const Event<T...>& other) { delegates.clear(); }
    void operator=(Event<T...>& other) { delegates.clear(); }
    
    Event(Event&& other) {
        delegates = other.delegates;
        other.delegates.clear();
    }
    
    ~Event() { Clear(); }
    
    void Clear() noexcept {
        for(auto d : delegates) delete d;
        delegates.clear();
    }
    
    bool Empty() const noexcept {
        return delegates.empty();
    }
    
    void operator () (T... values) {
        for(auto d : delegates) {
            d->Invoke(values...);
        }
    }
    
    template<typename Obj>
    void Bind(Obj* object, void (Obj::*method)(T...)) {
        IDelegateMember<Obj>* delegate = new IDelegateMember<Obj>();
        delegate->type = GetObjectID<Obj>();
        delegate->object = object;
        delegate->method = method;
        delegates.push_back(delegate);
    }
    
    template<typename Obj>
    void Unbind(Obj* object, void (Obj::*method)(T...)) {
        short typeID = GetObjectID<Obj>();
        for(int i=0; i<delegates.size(); ++i) {
            auto d = delegates[i];
            if (d->type != typeID) continue;
            IDelegateMember<Obj>* delegate = (IDelegateMember<Obj>*)(d);
            if (delegate->object != object) continue;
            if (delegate->method != method) continue;
            delegates.erase(delegates.begin() + i);
            return;
        }
    }
    
    template<typename Obj, typename Context>
    void Bind(Obj* object, void (Obj::*method)(T..., Context), Context context) {
        IDelegateMemberContext<Obj, Context>* delegate = new IDelegateMemberContext<Obj, Context>();
        delegate->type = 10000 + GetObjectID<Obj>() + GetObjectID<Context>() * 10000;
        delegate->object = object;
        delegate->method = method;
        delegate->context = context;
        delegates.push_back(delegate);
    }
    
    template<typename Obj, typename Context>
    void Unbind(Obj* object, void (Obj::*method)(T..., Context), Context context) {
        int typeID = 10000 + GetObjectID<Obj>() + GetObjectID<Context>() * 10000;
        for(int i=0; i<delegates.size(); ++i) {
            auto d = delegates[i];
            if (d->type != typeID) continue;
            IDelegateMemberContext<Obj, Context>* delegate = (IDelegateMemberContext<Obj, Context>*)(d);
            if (delegate->object != object) continue;
            if (delegate->method != method) continue;
            if (delegate->context != context) continue;
            delegates.erase(delegates.begin() + i);
            return;
        }
    }
    
    template<typename Lambda>
    void Bind(Lambda&& lambda) {
        IDelegateLambda* delegate = new IDelegateLambda();
        delegate->type = -1;
        delegate->function = lambda;
        delegates.push_back(delegate);
    }
};

template<typename...T>
int Event<T...>::objectIDCounter = 0;
}