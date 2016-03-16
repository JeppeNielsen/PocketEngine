#include "Engine.hpp"
#include "Window.hpp"
#include <iostream>
#include "Timer.hpp"
#include "../Debugging/Assert.hpp"
#include "OpenGL.hpp"
using namespace Pocket;

Engine::Engine() {
	window = 0;
    timer = new Timer();
}

Engine::~Engine() {
	if (window) {
		window->Destroy();
		delete window;
	}
    delete timer;
}

void Engine::StartLoop(IGameState* rootState, int width, int height, bool fullScreen) {
    this->rootState = rootState;
    this->fullScreen = fullScreen;
	this->window = Window::CreatePlatformWindow();
	this->window->Create(width, height, fullScreen);
    manager.screenSize = Vector2((float)width, (float)height);
	//this->rootState->DoInitialize(&manager);
    firstFrame = true;
    window->MainLoop.Bind(this, &Engine::Loop);
	window->Begin();
}

void Engine::Loop(bool* exit) {
        
    static float targetDelta = 1.0f / 60.0f;
    
    float delta = (float)timer->End();
    if (firstFrame) {
        manager.screenSize = window->screenSize;
        this->rootState->DoInitialize(&manager);
        delta = targetDelta;
        firstFrame = false;
    } else {
        if (delta>0.5f) delta = 0.5f;
    }
    if (window->ResetDeltaTime) {
        window->ResetDeltaTime = false;
        delta = targetDelta;
    }
    timer->Begin();
    //if (gameStateManager->CheckForChanges()) {
    //    (*exit) = true;
    //    return;
    //}
    
    if(!Update(delta)) {
        (*exit) = true;
        return;
    }
    
    Render();
}


bool Engine::Update(float dt) {
	bool running = window->Update(rootState);
	rootState->DoUpdate(dt);
	return running;
}


void Engine::Render() {
    ASSERT_GL(glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    ASSERT_GL(glEnable(GL_CULL_FACE));
	window->PreRender();
	rootState->DoRender();
	window->PostRender();
}





