#include "Window.hpp"

#ifdef IPHONE

#include "WindowiOS.hpp"

#elif OSX

#include "WindowOSX.hpp"

#elif EMSCRIPTEN

#include "WindowWeb.hpp"

#else

#include "WindowWin.hpp"

#endif

using namespace Pocket;

Window::Window() {
}

Window::~Window() {
}

void Window::Create(int width, int height, bool fullScreen) {
}

bool Window::Update(IInputManagerIterator* inputManagers) {
	return false;
}

void Window::Destroy() {}
void Window::PreRender() {}
void Window::PostRender() {}

Window* Window::CreatePlatformWindow() {
#ifdef IPHONE
    return new WindowiOS();
#elif OSX
    return new WindowOSX();
#elif EMSCRIPTEN
    return new WindowWeb();
#else
    return new WindowWin();
#endif
    
}

bool Window::Step() {
    bool exit = false;
    MainLoop(&exit);
    return exit;
}

void Window::Begin() {
    while (!Step()) {
        
    }
}

unsigned int Window::Framebuffer = 0;
