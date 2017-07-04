//
//  TypeInterpolator.hpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 03/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#pragma once
#include <type_traits>
#include "Quaternion.hpp"
#include "Colour.hpp"
#include "Property.hpp"
#include "Matrix4x4.hpp"
#include "Rect.hpp"
#include "Box.hpp"

namespace Pocket {

    template<class T, typename S = void>
    struct TypeInterpolator {
        static void Interpolate(T* value, float t, const T& a, const T& b) {
            *value = t<0.5f ? a : b;
        }
    };

    template<>
    struct TypeInterpolator<float> {
        static void Interpolate(float* value, float t, const float& a, const float& b) {
            *value = a + (b-a) * t;
        }
    };
    
    template<>
    struct TypeInterpolator<double> {
        static void Interpolate(double* value, float t, const double& a, const double& b) {
            *value = a + (b-a) * t;
        }
    };
    
    template<>
    struct TypeInterpolator<int> {
        static void Interpolate(int* value, float t, const int& a, const int& b) {
            *value = a + (b-a) * t;
        }
    };
    
    template<>
    struct TypeInterpolator<Vector2> {
        static void Interpolate(Vector2* value, float t, const Vector2& a, const Vector2& b) {
            *value = Vector2::Lerp(a, b, t);
        }
    };
    
    template<>
    struct TypeInterpolator<Vector3> {
        static void Interpolate(Vector3* value, float t, const Vector3& a, const Vector3& b) {
            *value = Vector3::Lerp(a, b, t);
        }
    };


    template<>
    struct TypeInterpolator<Quaternion> {
        static void Interpolate(Quaternion* value, float t, const Quaternion& a, const Quaternion& b) {
            *value = Quaternion::Slerp(t, a, b);
        }
    };
    
    template<>
    struct TypeInterpolator<Colour> {
        static void Interpolate(Colour* value, float t, const Colour& a, const Colour& b) {
            *value = Colour::Lerp(a, b, t);
        }
    };
    
    template<>
    struct TypeInterpolator<Matrix4x4> {
        static void Interpolate(Matrix4x4* value, float t, const Matrix4x4& a, const Matrix4x4& b) {
            *value = Matrix4x4::Lerp(a,b, t);
        }
    };
    
    template<>
    struct TypeInterpolator<Rect> {
        static void Interpolate(Rect* value, float t, const Rect& a, const Rect& b) {
            *value = Rect::Lerp(a, b, t);
        }
    };
    
    template<>
    struct TypeInterpolator<Box> {
        static void Interpolate(Box* value, float t, const Box& a, const Box& b) {
            *value = Box::Lerp(a, b, t);
        }
    };
    
    template<typename T>
    struct TypeInterpolator<Property<T>> {
        static void Interpolate(Property<T>* value, float t, const Property<T>& a, const Property<T>& b) {
            T v;
            TypeInterpolator<T>::Interpolate(&v, t, a(), b());
            value->operator=(v);
        }
    };
    
    template<typename T>
    struct TypeInterpolator<T, typename std::enable_if< std::is_enum<T>::value >::type> {
        static void Interpolate(T* value, float t, const T& a, const T& b) {
            int aVal = static_cast<int>(a);
            int bVal = static_cast<int>(b);
            float val = aVal + (float)(bVal - aVal) * t;
            int intVal = (int)val;
            *value = (T)intVal;
        }
    };
}
