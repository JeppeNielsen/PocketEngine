//
//  ObjectConstructor.hpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 13/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "GameIDHelper.hpp"
#include <vector>

namespace Pocket {

struct IObjectConstructor {
    virtual ~IObjectConstructor() {}
    virtual void* Construct() = 0;
    virtual std::string& Id() = 0;
};

template<typename Object, typename Type>
struct ObjectConstructor : public IObjectConstructor {
    void* Construct() override {
        return new Object();
    }
    std::string& Id() override {
        return Pocket::GameIdHelper::GetClassName<Type>();
    }
};

#define CONSTRUCTOR_BASE(Type) \
static std::vector<Pocket::IObjectConstructor*>& GetConstructors() { \
    static std::vector<Pocket::IObjectConstructor*> constructors; \
    return constructors; \
} \
static Type* ConstructObject(const std::string& id) { \
    for(auto constructor : GetConstructors()) { \
        if (constructor->Id() == id) return static_cast<Type*>(constructor->Construct()); \
    } \
    return nullptr; \
} \
virtual std::string& Id() = 0; \

#define CONSTRUCTOR_DERIVED(TypeName, TemplateName) \
private: \
    static struct ConstructorInitializer { \
        ConstructorInitializer() { \
            GetConstructors().push_back(new Pocket::ObjectConstructor<TypeName<TemplateName>, TemplateName>()); \
        } \
    } constructorInitializer; \
public: \
    TypeName<TemplateName>() { \
        constructorInitializer; \
    } \
    std::string& Id() override { \
        return Pocket::GameIdHelper::GetClassName<TemplateName>(); \
    }

#define CONSTRUCTOR_DERIVED_INITIALIZER(TypeName) \
template<typename T> typename TypeName<T>::ConstructorInitializer TypeName<T>::constructorInitializer;

}
