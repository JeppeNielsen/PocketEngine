//
//  Span.h
//  PocketEngine
//
//  Created by Jeppe Nielsen on 9/15/13.
//  Copyright (c) 2013 Jeppe Nielsen. All rights reserved.
//

namespace Pocket {
    
    template<typename T>
    class Span {
    public:
        Span();
        ~Span();
        
        Span(const T& values);
        Span(const T& min, const T& max);
        
        Span(const Span<T>& other);
        
        operator const T& () const;
        
        T Min;
        T Max;
        
        T Get(const float& t) const;
    };
}

template<typename T>
Pocket::Span<T>::Span() {}

template<typename T>
Pocket::Span<T>::~Span() {}

template<typename T>
Pocket::Span<T>::Span(const Span<T>& other) : Min(other.Min), Max(other.Max) { }

template<typename T>
Pocket::Span<T>::Span(const T& values) : Min(values), Max(values) { }

template<typename T>
Pocket::Span<T>::Span(const T& min, const T& max) : Min(min), Max(max) { }

template<typename T>
T Pocket::Span<T>::Get(const float &t) const {
    return Min + (Max - Min) * t;
}

template<typename T>
Pocket::Span<T>::operator const T& () const {
    return Min;
}
