//
//  TestBigButtons.cpp
//  PocketEditor
//
//  Created by Jeppe Nielsen on 26/11/2017.
//  Copyright © 2017 Jeppe Nielsen. All rights reserved.
//

#include "Engine.hpp"
#include "BigButtonManager.hpp"

using namespace Pocket;

class TestBigButtons : public GameState<TestBigButtons> {
public:
    BigButtonManager bigButtons;
    
    void Initialize() {
        bigButtons.Initialize();
        bigButtons.ButtonDown.Bind([] (BigButtonEvent e) {
            std::cout << "bla"<< (int)e.Button << std::endl;
        });
    }
    
    void Update(float dt) {
        bigButtons.Update(dt);
    }
    
    void Render() {
    }
};

int main_bigbuttons() {
    Engine e;
    e.Start<TestBigButtons>();
    return 0;
}
