//
//  Event.hpp
//  EntitySystem
//
//  Created by Jeppe Nielsen on 07/12/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <vector>
#include <functional>

namespace Pocket {

template<class... T>
class Event {
private:
    using Signature = void(T...);
    using Delegate = std::function<Signature>;
    using Delegates = std::vector<Delegate>;
    Delegates delegates;
    
    template<typename Obj, typename ...Args>
    struct MemberDelegate {
        Obj* x;
        void (Obj::*f)(Args...);

        template<typename ...Ts>
        void operator()(Ts&&... args) {
            (x->*f)(std::forward<Ts>(args)...);
        }
    };
    
    template<typename Obj, typename Context, typename ...Args>
    struct MemberDelegateContext {
        Obj* x;
        Context context;
        void (Obj::*f)(Args..., Context);

        template<typename ...Ts>
        void operator()(Ts&&... args) {
            (x->*f)(std::forward<Ts>(args)..., context);
        }
    };
    
public:
    
    void operator () (T... values) {
        for(auto& d : delegates) {
            d(values...);
        }
    }
    
    void Clear() noexcept {
        delegates.clear();
    }
    
    bool Empty() const noexcept {
        return delegates.empty();
    }
    
    void Bind(Delegate&& delegate) {
        delegates.push_back(delegate);
    }
    
    void Unbind(Delegate&& delegate) {
        auto& delegateType = delegate.target_type();
        auto delegateTarget = delegate.template target<Signature>();
        for (auto i = 0; i<delegates.size(); ++i) {
            auto d = delegates[i];
            auto& type = d.target_type();
            if (type != delegateType) continue;
            auto dTarget = d.template target<Signature>();
            if ( delegateTarget == dTarget) {
                delegates.erase(delegates.begin() + i);
                break;
            }
        }
    }
    
    template<typename Obj>
    void Bind(Obj* x, void (Obj::*fun)(T...)) {
        MemberDelegate<Obj, T...> delegate{x, fun};
        Bind(delegate);
    }
    
    template<typename Obj>
    void Unbind(Obj* x, void (Obj::*fun)(T...)) {
        MemberDelegate<Obj, T...> delegate{x, fun};
        Unbind(delegate);
    }
    
    template<typename Obj, typename Context>
    void Bind(Obj* x, void (Obj::*fun)(T..., Context), Context context) {
        MemberDelegateContext<Obj, Context, T...> delegate{x, context, fun};
        Bind(delegate);
    }
    
    template<typename Obj, typename Context>
    void Unbind(Obj* x, void (Obj::*fun)(T..., Context), Context context) {
        MemberDelegateContext<Obj, Context, T...> delegate{x, context, fun};
        Unbind(delegate);
    }
};

}