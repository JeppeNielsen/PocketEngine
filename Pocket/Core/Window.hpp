#pragma once
#include "InputManager.hpp"
#include "Event.hpp"
#include "Vector2.hpp"

namespace Pocket {
	class Window
	{
	public:
		Window();
		virtual ~Window();

		virtual void Create(int width, int height, bool fullScreen);
		virtual void Destroy();
		virtual bool Update(IInputManagerIterator* inputManagers);
		virtual void PreRender();
		virtual void PostRender();
        virtual void Begin();

		static Window* CreatePlatformWindow();
        
        Event<bool*> MainLoop;
        
        Vector2 screenSize;
        
        static unsigned int Framebuffer;
        
    protected:
        
        bool Step();

	};
}

