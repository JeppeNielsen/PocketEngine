//
//  testIsBaseOf.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 06/09/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include <type_traits>
using namespace std;

class Interface {
    public :
       virtual void Method() = 0;
       virtual ~Interface() { }
};

class Derived : public Interface {
    public : 
       void Method() {
            cout<<"Interface method"<<endl;
       }
};

template<typename T, typename S = void> 
struct Selector {
    static void Select(T& o) {
        std::cout << o;
        cout<<"Generic method"<<endl;
    }
};

template<typename T>
struct Selector<T, typename enable_if< is_base_of<Interface, T>::value >::type> {
    static void Select(Interface& o) {
        o.Method();
    }
};

int main_nono()
{
int i;
Selector<int>::Select(i);       // prints out "Generic method" -> ok
Derived d;
Selector<Derived>::Select(d);  // prints out "Generic method" -> wrong

return 0;
}