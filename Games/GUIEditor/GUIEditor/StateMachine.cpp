//
//  StateMachine.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 07/10/15.
//  Copyright (c) 2015 Jeppe Nielsen. All rights reserved.
//

#include <functional>
#include <vector>
#include <iostream>

struct StateMachine {

    int i;
    
  std::vector<std::function<bool()>> stateMachine() {

    i = 0;
    return
    {
        [&] () -> bool {
            if (i<10) {
                return true;
            } else {
                i++;
            }
            return false;
        },
        [&] () -> bool {
            i++;
            return true;
        }
    };
    }
    
    typedef std::vector<std::function<bool()>> States;
    
    typedef std::function<States()> Machine;
    
    
    States states;
    
    void Begin(Machine machine) {
        states = machine();
    }
    
    bool Step() {
        if (states.empty()) {
            return false;
        }
        bool result = states.front()();
        if (result) {
            states.erase(states.begin());
        }
        return true;
    }
};


void TestStateMachine(StateMachine& m, StateMachine& m2) {

    int i = 0;
    StateMachine::Machine machine = [&] () -> StateMachine::States {
        i=0;
        static int myCounter =0;
        return {
            [&] () -> bool {
                i++;
                std::cout << "counting one up"<<std::endl;
                if (i<10) {
                    return false;
                }
                return true;
            },
            [&] () -> bool {
                
                if (myCounter++<10) {
                    std::cout<<"tick"<<std::endl;
                    return false;
                }
            
                std::cout<<"i == " << i<<std::endl;
                return true;
            }
        };
    };
    
    
    
    StateMachine::Machine machine2 = [&] () -> StateMachine::States {
        static int counter = 0;
        return {
            [&] () -> bool {
                i++;
                std::cout << "counting one up from machine 2"<<std::endl;
                if (i<3) {
                    return false;
                }
                return true;
            },
            [&] () -> bool {
                counter++;
                std::cout << "Counting from machine 2 :"<< counter<<std::endl;
                return counter>=5;
            },
            [&] () -> bool {
                std::cout<<"i == " << i<<std::endl;
                return true;
            }
        };
    };

    m.Begin(machine);
    m2.Begin(machine2);
}



int main() {
    
    for (int i=0; i<2; i++) {
    
    StateMachine m;
    StateMachine m2;
    TestStateMachine(m, m2);
    
    while (true) {
        bool s1 = m.Step();
        bool s2 = m2.Step();
        if (!s1 && !s2) break;
    }
    std::cout<< "----------"<<std::endl;
    }
    
    return 0;
}