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
    staticContext = &context;
}

Engine::~Engine() {
	if (window) {
		window->Destroy();
		delete window;
	}
    delete timer;
    staticContext = 0;
}

void Engine::StartLoop(IGameState* rootState, int width, int height, bool fullScreen) {
    this->rootState = rootState;
    this->fullScreen = fullScreen;
	this->window = Window::CreatePlatformWindow();
	this->window->context = &context;
	this->window->Create(width, height, fullScreen);
    firstFrame = true;
    window->MainLoop.Bind(this, &Engine::Loop);
	window->Begin();
}

void Engine::Loop(bool* exit) {
    
    
    static float targetDelta = 1.0f / 60.0f;
    
    float delta = (float)timer->End();
    if (firstFrame) {
        this->rootState->DoInitialize(&context);
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
    
    if(!Update(delta)) {
        (*exit) = true;
        return;
    }
    
    Render();
}


bool Engine::Update(float dt) {
    context.InputDevice().StartFrame(rootState);
	bool running = window->Update();
    rootState->DoUpdate(dt);
	context.InputDevice().EndFrame();
    return running;
}

void Engine::Render() {
#ifdef ASSERT_GL
    ASSERT_GL(glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    ASSERT_GL(glEnable(GL_CULL_FACE));
#else
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
#endif
	window->PreRender();
	rootState->DoRender();
	window->PostRender();
}

EngineContext& Engine::Context() { return *staticContext; }

EngineContext* Engine::staticContext = 0;

bool Engine::HasContext() { return staticContext; }