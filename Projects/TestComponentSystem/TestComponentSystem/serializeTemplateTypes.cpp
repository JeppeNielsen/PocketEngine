//
//  testPolymorficType.cpp
//  TestComponentSystem
//
//  Created by Jeppe Nielsen on 06/07/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include <stdio.h>
#include "TypeInfo.hpp"
#include <sstream>
#include <map>

using namespace Pocket;

template<typename T>
struct List;

struct IList {
    virtual ~IList() {}
    virtual TypeInfo GetType() = 0;
    virtual void Print() = 0;
    CONSTRUCTOR_BASE(IList)
};

template<typename T>
struct List : public IList {

    std::vector<T> data;

    TypeInfo GetType() override {
        TypeInfo info;
        info.AddField(data, "data");
        return info;
    }
    
    void Print() override {
        std::cout << " Printing: " <<std::endl;
        for(auto d : data) {
            std::cout << d << std::endl;
        }
    }
    
    CONSTRUCTOR_DERIVED(List, T)
};

CONSTRUCTOR_DERIVED_INITIALIZER(List)

struct ListContainer {
    
    std::vector<std::unique_ptr<IList>> lists;

    TYPE_FIELDS_BEGIN
    TYPE_FIELD(lists)
    TYPE_FIELDS_END
};

int main() {

    std::unique_ptr<List<int>> intList = std::make_unique<List<int>>();
    intList->data = {1,2,3,4};
    
    std::unique_ptr<List<float>> floatList = std::make_unique<List<float>>();
    floatList->data = {0.1f,0.2f,0.3f,0.4f};
    
    std::unique_ptr<List<std::string>> textList = std::make_unique<List<std::string>>();
    textList->data = {"Jeppe", "Er", "God"};
    
    ListContainer container;
    container.lists.push_back(std::move(intList));
    container.lists.push_back(std::move(floatList));
    container.lists.push_back(std::move(textList));
    
    for(auto& l : container.lists) {
        l->Print();
    }

    auto type = container.GetType();


{
    std::stringstream s;
    minijson::writer_configuration config;
    config = config.pretty_printing(true);
    minijson::object_writer writer(s, config);
    type.Serialize(writer);
    writer.close();
    
    std::cout << s.str() << std::endl;
    
    
    minijson::istream_context context(s);
    
    ListContainer container2;
    auto type2 = container2.GetType();
    
    type2.Deserialize(context);
    
    
    for(auto& l : container2.lists) {
        l->Print();
    }

    
    
    minijson::object_writer writer2(std::cout, config);
    type2.Serialize(writer2);
    writer2.close();
}




    return 0;
}
