//
//  main.cpp
//  BuildWithClang
//
//  Created by Jeppe Nielsen on 05/07/16.
//  Copyright © 2016 Jeppe Nielsen. All rights reserved.
//

#include "OpenGL.hpp"
#include "Engine.hpp"

using namespace Pocket;

struct Game : public GameState<Game> {
    void Initialize() { }
    void Update(float dt) {}
    
    void Render() {
        glClearColor(1,0,0,0);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
};

int main(int argc, const char * argv[]) {
    Engine e;
    e.Start<Game>();
    return 0;
}
