#pragma once
#include <windows.h>
#include "Window.hpp"
#include "InputDevice.hpp"
#include "OpenGL.hpp"
#include <map>
#include <EGL/egl.h>



namespace Nano {
	class WindowWin : public Nano::Window
	{
		public:

			
			void Create(int width, int height, bool fullScreen);
			void Destroy();
			bool Update(InputManager& input);
			void PreRender();
			void PostRender();
			bool closed;
			InputDevice inputDevice;

			std::string* GetKey(int code);

			bool active;

		private:
			HWND window;
			HDC hDC;
			HGLRC hRC;

			// EGL variables
			EGLDisplay			eglDisplay;
			EGLConfig			eglConfig;
			EGLSurface			eglSurface;
			EGLContext			eglContext;
			NativeWindowType	eglWindow;
			EGLint				pi32ConfigAttribs[128];
			GLuint	ui32Vbo;


			typedef std::map<int, std::string> KeyMap;
			KeyMap keyMap;

			void InitKeyMap();

			void EnableOpenGL();

			void DisableOpenGL();
			
	};
}
