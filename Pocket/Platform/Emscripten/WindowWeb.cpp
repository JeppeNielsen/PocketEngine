//
//  WindowWeb.cpp
//  GUIEditor
//
//  Created by Jeppe Nielsen on 3/31/14.
//  Copyright (c) 2014 Jeppe Nielsen. All rights reserved.
//

#include "WindowWeb.hpp"
#include <emscripten.h>
#include <iostream>

using namespace Pocket;
using namespace std;

static WindowWeb* instance;

void step() {
    instance->Loop();
}

void WindowWeb::Create(int width, int height, bool fullScreen) {
    instance = this;
    frameRate = 60;
    if (SDL_Init (SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) == 0) {
		
		SDL_Surface* screen = SDL_SetVideoMode (width, height, 0, SDL_OPENGL);
		
		if (screen != NULL) {
			
			previousTime = 0;
			active = true;
			paused = false;
            context->ScreenSize = Vector2(width, height);
            inputDevice().Initialize(3);
		} else {
			cerr << "Could not set video mode: " << SDL_GetError () << endl;
		}
	} else {
		cerr << "Could not initialize SDL: " << SDL_GetError () << endl;
	}
}

void WindowWeb::Destroy() {
    SDL_Quit ();
}

bool WindowWeb::Update() {
    SDL_Event event;
    while (SDL_PollEvent (&event)) {
        handleEvent (event);
    }
    //inputDevice.Update(inputManagers);
    return true;
}

void WindowWeb::PreRender() {
    const Vector2& screenSize = context->ScreenSize;
    glViewport(0, 0, screenSize.x, screenSize.y);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void WindowWeb::PostRender() {
    SDL_GL_SwapBuffers ();
}

void WindowWeb::Begin() {
    emscripten_set_main_loop (step, 0, true);
}

void WindowWeb::Loop() {
    while (Step()) {
        
    }
}

Vector2 WindowWeb::ConvertMousePosition(int x, int y) {
    return Vector2(x, context->ScreenSize().y - y);
}

int WindowWeb::GetMouseButtonIndex(int index) {
    return index == SDL_BUTTON_LEFT ? 0 : (index == SDL_BUTTON_RIGHT ? 1 : 2);
}

void WindowWeb::handleEvent (SDL_Event &event) {
    
    switch (event.type)
    {
        case SDL_MOUSEBUTTONDOWN: {
            inputDevice().SetTouch(GetMouseButtonIndex(event.button.button), true, ConvertMousePosition(event.button.x,event.button.y));
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            inputDevice().SetTouch(GetMouseButtonIndex(event.button.button), false, ConvertMousePosition(event.button.x,event.button.y));
            break;
        }
        case SDL_MOUSEMOTION: {
            Vector2 position = ConvertMousePosition(event.motion.x,event.motion.y);
            for (int i=0; i<4; i++) {
                inputDevice().SetTouchPosition(i, position);
            }
            break;
        }
        case SDL_KEYDOWN: {
            std::string s;
            s.assign(1, event.key.keysym.sym);
            inputDevice().SetButton(s, true);
            break;
        }
        case SDL_KEYUP: {
            std::string s;
            s.assign(1, event.key.keysym.sym);
            inputDevice().SetButton(s, false);
            break;
        }
    }
    
    event.type = -1;
}

InputDevice& WindowWeb::inputDevice() {
    return context->InputDevice();
}