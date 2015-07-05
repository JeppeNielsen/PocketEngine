//
//  main.cpp
//  Android
//
//  Created by Jeppe Nielsen on 03/07/15.
//
//

#include "Engine.hpp"
#include "OpenGl.hpp"

using namespace Pocket;

class Android : public GameState<Android> {
public:

    float timer;
    void Initialize() {
        timer = 0;
    }

    void Update(float dt) {
        timer+=dt;
        if (timer>1.0f) {
            timer = 0.0f;
        }
    }

    void Render() {
        glClearColor(timer,timer,0);
    }
};

int main() {
    Engine e;
    e.Start<Android>();
    return 0;
}
