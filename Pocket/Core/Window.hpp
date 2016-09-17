#pragma once
#include "InputManager.hpp"
#include "Event.hpp"
#include "Property.hpp"
#include "Vector2.hpp"
#include "EngineContext.hpp"

namespace Pocket {
	class Window
	{
	public:
		Window();
		virtual ~Window();

		virtual void Create(int width, int height, bool fullScreen);
		virtual void Destroy();
		virtual bool Update();
		virtual void PreRender();
		virtual void PostRender();
        virtual void Begin();

		static Window* CreatePlatformWindow();
        
        Event<bool*> MainLoop;
        
        EngineContext* context;
        
        static unsigned int Framebuffer;
        
        bool ResetDeltaTime;
        
    protected:
        
        bool Step();

	};
}

