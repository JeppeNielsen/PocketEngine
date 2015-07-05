//
//  WindowAndroid.cpp
//  Android
//
//  Created by Jeppe Nielsen on 03/07/15.
//
//

#include "WindowAndroid.hpp"

using namespace Pocket;
using namespace std;

static WindowAndroid* instance;

void step() {
    instance->Loop();
}

void WindowAndroid::Create(int width, int height, bool fullScreen) {
 /*
    instance = this;
    frameRate = 60;
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0) {
		
		SDL_Surface* screen = SDL_SetVideoMode (width, height, 0, SDL_OPENGL);
		
		if (screen != NULL) {
			
			previousTime = 0;
			active = true;
			paused = false;
            screenSize = Vector2(width, height);
            inputDevice.Initialize(3);
			
		} else {
			cerr << "Could not set video mode: " << SDL_GetError () << endl;
		}
	} else {
		cerr << "Could not initialize SDL: " << SDL_GetError () << endl;
	}
    */
}

void WindowAndroid::Destroy() {
    
}

bool WindowAndroid::Update(IInputManagerIterator* inputManagers) {
    /*SDL_Event event;
    while (SDL_PollEvent (&event)) {
        handleEvent (event);
    }
    */
    inputDevice.Update(inputManagers);
    return true;
}

void WindowAndroid::PreRender() {
    glViewport(0, 0, screenSize.x, screenSize.y);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void WindowAndroid::PostRender() {
    //SDL_GL_SwapBuffers ();
}

void WindowAndroid::Begin() {
   // emscripten_set_main_loop (step, 0, true);
}

void WindowAndroid::Loop() {
    while (Step()) {
        
    }
}

Vector2 WindowAndroid::ConvertMousePosition(int x, int y) {
    return Vector2(x, screenSize.y - y);
}

int WindowAndroid::GetMouseButtonIndex(int index) {
    return 0;//index == SDL_BUTTON_LEFT ? 0 : (index == SDL_BUTTON_RIGHT ? 1 : 2);
}
