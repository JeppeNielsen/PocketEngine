//
//  testInitializer.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 12/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include <map>
#include <string>
#include "GameIDHelper.hpp"
#include <vector>
#include "ObjectConstructor.hpp"

//
//struct IObjectConstructor {
//    virtual ~IObjectConstructor() {}
//    virtual void* Construct() = 0;
//    virtual std::string& Id()=0;
//};
//
//template<typename Object, typename Type>
//struct ObjectConstructor : public IObjectConstructor {
//    void* Construct() override {
//        return new Object();
//    }
//    std::string& Id() override {
//        return Pocket::GameIdHelper::GetClassName<Type>();
//    }
//};
//
//#define BASE_CONSTRUCTOR(Type) \
//static std::vector<Pocket::IObjectConstructor*>& GetConstructors() { \
//    static std::vector<Pocket::IObjectConstructor*> constructors; \
//    return constructors; \
//} \
//static Type* Create(const std::string& id) { \
//    for(auto constructor : GetConstructors()) { \
//        if (constructor->Id() == id) return static_cast<Type*>(constructor->Construct()); \
//    } \
//    return nullptr; \
//} \
//virtual std::string& Id() = 0; \
//
//#define DERIVED_CONSTRUCTOR(TypeName, TemplateName) \
//private: \
//    static struct ConstructorInitializer { \
//        ConstructorInitializer() { \
//            GetConstructors().push_back(new Pocket::ObjectConstructor<TypeName<TemplateName>, TemplateName>()); \
//        } \
//    } constructorInitializer; \
//public: \
//    TypeName<TemplateName>() { \
//        constructorInitializer; \
//    } \
//    std::string& Id() override { \
//        return Pocket::GameIdHelper::GetClassName<TypeName>(); \
//    }
//
//#define DERIVED_CONSTRUCTOR_INITIALIZER(TypeName, TemplateName) \
//template<typename T> typename Derived<T>::ConstructorInitializer Derived<T>::constructorInitializer;
//


struct IBase {
    virtual ~IBase() {}
    CONSTRUCTOR_BASE(IBase)
};

template<typename T>
struct Derived  : public IBase {
    CONSTRUCTOR_DERIVED(Derived, T)
};

CONSTRUCTOR_DERIVED_INITIALIZER(Derived)

struct TesterClass {
    Derived<std::string> member;
};

int main_initializer() {

    for(auto c : IBase::GetConstructors()) {
        std::cout << c->Id() << std::endl;
    }
    
    
//    for(int i=0; i<IBase::typePointersCounter; i++) {
//        std::cout << IBase::typePointers[i]->Id() << std::endl;
//    }
    
    TesterClass ghy;
    
    IBase* intList = new Derived<int>();
    IBase* floatList = new Derived<float>();
    IBase* floatList2 = new Derived<float>();
    IBase* floatList3 = new Derived<float>();
    
    
    IBase* numberList = IBase::ConstructObject("int");
    IBase* numberFloatList = IBase::ConstructObject("float");
    IBase* matrixList = IBase::ConstructObject("MatrixWolla");
    
    
    std::cout << numberList->Id() << std::endl;
    std::cout << numberFloatList->Id() << std::endl;
    
    IBase* myList = new Derived<Pocket::GameIdHelper>();
    


    return 0;
}
