
#include <iostream>
#include <tuple>
#include "MetaLibrary.hpp"
//
//namespace Meta {
//
//template <typename... >
//struct index;
//
//// found it
//template <typename T, typename... R>
//struct index<T, T, R...>
//: std::integral_constant<size_t, 0>
//{ };
//
//// still looking
//template <typename T, typename F, typename... R>
//struct index<T, F, R...>
//: std::integral_constant<size_t, 1 + index<T,R...>::value>
//{ };
//
//
//
//template <typename T>
//constexpr size_t ordinal() { return index<T, int, float, long>::value; }
//
//}

template<typename T>
struct Vector;

struct IVector {
    virtual void Print() = 0;
    
    using Types = std::tuple<Vector<int>, Vector<float>, Vector<long>>;
};

template<typename T>
struct Vector : public IVector {
    T val;

    void Print() override {
        std::cout << typeid(T).name() << std::endl;
    }
};

//template<typename Base, typename Types>
//Base* CreateType(const int typeId) {
//    const static Types serializableTypes;
//    IVector* ret = nullptr;
//    int n = 0;
//    Pocket::for_each_in_tuple(serializableTypes, [&] (auto p) {
//        if (n == typeId) {
//           using Type = std::remove_pointer_t< decltype(p) >;
//           ret = new Type();
//        }
//        n++;
//    });
//    return ret;
//}

int main_constExpr() {

    //auto types = AddType<std::string>();


//    std::cout << Pocket::IndexInTuple<int*, decltype(SerializableTypes)>::value << std::endl;
//    std::cout << Pocket::IndexInTuple<float*, decltype(SerializableTypes)>::value << std::endl;
//    std::cout << Pocket::IndexInTuple<long*, decltype(SerializableTypes)>::value << std::endl;
//    std::cout << Pocket::IndexInTuple<std::string, decltype(SerializableTypes)>::value << std::endl;
    //std::cout << Meta::ordinal<int>() << std::endl;
    
    
//    IVector* type1 = Pocket::IndexToType<IVector, IVector::Types>(0);
//    IVector* type2 = Pocket::IndexToType<IVector, IVector::Types>(1);
//    
//    type1->Print();
//    type2->Print();
//    
//    Vector<float>* floatVector = new Vector<float>();
//    Vector<int>* intVector = new Vector<int>();
//    
//    
//    int index1 = Pocket::IndexInTuple<std::remove_pointer_t<decltype(floatVector)>, IVector::Types>::value;
//    int index2 = Pocket::IndexInTuple<std::remove_pointer_t<decltype(intVector)>, IVector::Types>::value;
//    
//    
//    std::cout << index1 << std::endl;
//    std::cout << index2 << std::endl;
//    
    
    
//    
//    
//    IVector* intList = IVector::Create(0);
//    IVector* floatList = IVector::Create(1);
//    IVector* longList = IVector::Create(2);
//    
//    IVector* nullList = IVector::Create(10);
    
    
    return 0;
}

