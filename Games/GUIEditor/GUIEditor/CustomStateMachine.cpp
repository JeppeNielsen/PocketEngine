//
//  CustomStateMachine.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 08/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include <iostream>
#include <functional>
#include <vector>
#include <string>
#include <map>

struct IStateVariable {

};

template<class T>
struct StateVariable : public IStateVariable {
    T value;
};

struct State {
    State(std::function<bool()> code) {
        this->code = code;
    }

    void Reset() {
        currentVar = 0;
    }
    
    template<class T>
    StateVariable<T>* GetData(bool& created) {
        StateVariable<T>* var;
        if (currentVar>=variables.size()) {
            var = new StateVariable<T>();
            variables.push_back(var);
            created = true;
        } else {
            var = (StateVariable<T>*)variables[currentVar];
            created = false;
        }
        currentVar++;
        return var;
    }
    
    template<class T>
    StateVariable<T>* GetData(T v) {
        bool created;
        StateVariable<T>* variable = GetData<T>(created);
        if (created) {
            variable->value = v;
        }
        return variable;
    }
    std::function<bool()> code;
    int currentVar;
    std::vector<IStateVariable*> variables;
    
    static State* currentState;
};

State* State::currentState = 0;

template<class T>
class Var {
public:
    Var() {
        bool created;
        value = &State::currentState->GetData<T>(created)->value;
    }
    Var(T v) {
        value = &State::currentState->GetData<T>(v)->value;
    }
    
    operator const T () const { return *value; }
    T operator + (T value) const { return (*this->value) + value; }
    T operator - (T value) const { return (*this->value) - value; }
    T operator * (T value) const { return (*this->value) * value; }
    T operator / (T value) const { return (*this->value) / value; }
    T operator += (T value) { return (*this->value) += value; }
    T operator -= (T value) { return (*this->value) -= value; }
    T operator *= (T value) { return (*this->value) *= value; }
    T operator /= (T value) { return (*this->value) /= value; }
    //T operator ++ (T value) { return (*this->value)++; }
    //T operator -- (T value) { return (*this->value)--; }
    T operator - () const { return -(*value); }
    void operator = (T value) { (*this->value) = value; }
    
private:
    T* value;
};



typedef std::vector<std::function<bool()>> States;

class Machine {
public:
    
    void Start(States s) {
        for(auto code : s) {
            states.push_back(State(code));
        }
    }

    bool Step() {
        if (states.empty()) return false;
        State& state = states.front();
        state.Reset();
        State::currentState = &state;
        bool exit = state.code();
        if (!exit) {
            states.erase(states.begin());
        }
        return true;
    }

private:
    typedef std::vector<State> StateList;
    StateList states;
};

void StartMachine(Machine& m, Machine& m2) {
    {
        States states = {
            [&] () -> bool {
                Var<int> counter;
                counter+=1;
                std::cout << "State 1"<<std::endl;
                return counter<5;
            },
            [&] () -> bool {
                std::cout << "State 2"<<std::endl;
                return false;
            },
            [&] () -> bool {
                Var<float> timer(0.0f);
                timer+=0.1f;
                std::cout<<"state"<<std::endl;
                return timer<2.0f;
            }
        };
        m.Start(states);
    }
    
     {
        States states = {
            [&] () -> bool {
                Var<int> counter(5);
                counter+=1;
                std::cout << "State 1"<<std::endl;
                return counter<5;
            },
            [&] () -> bool {
                std::cout << "State 2"<<std::endl;
                return false;
            },
            [&] () -> bool {
                Var<float> timer(0.0f);
                timer+=0.1f;
                std::cout<<"state"<<std::endl;
                return timer<2.0f;
            }
        };
        m2.Start(states);
    }
}

int main__() {
    
    Machine m;
    Machine m2;
    StartMachine(m, m2);
    
    int c = 0;
    while (m.Step() && m2.Step()) {
        std::cout << c++ << std::endl;
    }
    return 0;
}


