//
//  TestStateMachine.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 08/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include <functional>
#include <vector>
#include <iostream>

enum class StateMode { Repeat, Next, Previous };
typedef std::function<StateMode()> State;
typedef std::vector<State> StateList;



template<class Data>
class Machine {
public:
    Machine() : currentState(0) { }

    void Start(Data data, StateList states) {
        this->data = data;
        this->states = states;
        currentState = 0;
    }
    
    bool Step() {
        if (currentState<0 || currentState>=states.size()) return false;
        StateMode mode = states[currentState]();
        if (mode == StateMode::Next) {
            currentState++;
        } else if (mode == StateMode::Previous) {
            currentState--;
        }
        return true;
    }
    
private:
    Data data;
    StateList states;
    int currentState;
};

int main() {

    int data = 0;
    for (int i=0; i<2; i++) {
    Machine<int> machine;
    machine.Start(data, {
        [&] () -> StateMode {
            static float timer = 0;
            timer += 1.0f;
            data++;
            return timer>10 ? StateMode::Next : StateMode::Repeat;
        },
        [&] () -> StateMode {
            data++;
            return StateMode::Next;
        }
    });
    
    while (machine.Step()) { }
    
    std::cout<< data<<std::endl;
    }
    
    
    

    return 0;
}
