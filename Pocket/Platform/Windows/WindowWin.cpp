#include "WindowWin.hpp"
#include "OpenGL.hpp"
#include <iostream>
#include <map>
#include "Vector2.hpp"

PFNGLMAPBUFFEROESPROC glMapBuffer;
PFNGLUNMAPBUFFEROESPROC glUnmapBuffer;

using namespace Pocket;


using namespace std;
using namespace Pocket;

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );

static std::map<HWND, WindowWin*> handleToClass;

std::string* WindowWin::GetKey(int code) {
	KeyMap::iterator it = keyMap.find(code);
	if (it==keyMap.end()) return 0;
	return &it->second;
}

void WindowWin::InitKeyMap() {

	keyMap[0x30] = "0";
	keyMap[0x31] = "1";
	keyMap[0x32] = "2";
	keyMap[0x33] = "3";
	keyMap[0x34] = "4";
	keyMap[0x35] = "5";
	keyMap[0x36] = "6";
	keyMap[0x37] = "7";
	keyMap[0x38] = "8";
	keyMap[0x39] = "9";


	keyMap[0x041] = "A";
	keyMap[0x042] = "B";
	keyMap[0x043] = "C";
	keyMap[0x044] = "D";
	keyMap[0x045] = "E";
	keyMap[0x046] = "F";
	keyMap[0x047] = "G";
	keyMap[0x048] = "H";
	keyMap[0x049] = "I";
	keyMap[0x04A] = "J";
	keyMap[0x04B] = "K";
	keyMap[0x04C] = "L";
	keyMap[0x04D] = "M";
	keyMap[0x04E] = "N";
	keyMap[0x04F] = "O";
	keyMap[0x050] = "P";
	keyMap[0x051] = "Q";
	keyMap[0x052] = "R";
	keyMap[0x053] = "S";
	keyMap[0x054] = "T";
	keyMap[0x055] = "U";
	keyMap[0x056] = "V";
	keyMap[0x057] = "W";
	keyMap[0x058] = "X";
	keyMap[0x059] = "Y";
	keyMap[0x05A] = "Z";
	
	keyMap[0x08] = "Back";
	keyMap[0x09] = "Tab";
	keyMap[0x0C] = "Clear";
	keyMap[0x0D] = "Enter";
	keyMap[0x10] = "Shift";
	keyMap[0x11] = "Ctrl";
	keyMap[0x12] = "Alt";
	keyMap[0x13] = "Pause";
	keyMap[0x14] = "CapsLock";
	keyMap[0x1B] = "Esc";
	keyMap[0x20] = "Space";
	keyMap[0x21] = "PageUp";
	keyMap[0x22] = "PageDown";
	keyMap[0x23] = "End";
	keyMap[0x24] = "Home";
	keyMap[0x25] = "Left";
	keyMap[0x26] = "Up";
	keyMap[0x27] = "Right";
	keyMap[0x28] = "Down";
	keyMap[0x29] = "Print";
	keyMap[0x2B] = "Execute";
	keyMap[0x2C] = "PrintScreen";
	keyMap[0x2D] = "Insert";
	keyMap[0x2E] = "Delete";
	keyMap[0x2F] = "Help";
	keyMap[0x5B] = "LeftWindows";
	keyMap[0x5C] = "RightWindows";
	keyMap[0x5D] = "Apps";
	keyMap[0x60] = "Numpad0";
	keyMap[0x61] = "Numpad1";
	keyMap[0x62] = "Numpad2";
	keyMap[0x63] = "Numpad3";
	keyMap[0x64] = "Numpad4";
	keyMap[0x65] = "Numpad5";
	keyMap[0x66] = "Numpad6";
	keyMap[0x67] = "Numpad7";
	keyMap[0x68] = "Numpad8";
	keyMap[0x69] = "Numpad9";

	keyMap[0x6A] = "Multiply";
	keyMap[0x6B] = "Add";
	keyMap[0x6C] = "Separator";
	keyMap[0x6D] = "Subtract";
	keyMap[0x6E] = "Decimal";
	keyMap[0x6F] = "Divide";

	keyMap[0x70] = "F1";
	keyMap[0x71] = "F2";
	keyMap[0x72] = "F3";
	keyMap[0x73] = "F4";
	keyMap[0x74] = "F5";
	keyMap[0x75] = "F6";
	keyMap[0x76] = "F7";
	keyMap[0x77] = "F8";
	keyMap[0x78] = "F9";
	keyMap[0x79] = "F10";
	keyMap[0x7A] = "F11";
	keyMap[0x7B] = "F12";
	keyMap[0x7C] = "F13";
	keyMap[0x7D] = "F14";
	keyMap[0x7E] = "F15";
	keyMap[0x7F] = "F16";
	keyMap[0x80] = "F17";
	keyMap[0x81] = "F18";
	keyMap[0x82] = "F19";
	keyMap[0x83] = "F20";
	keyMap[0x84] = "F21";
	keyMap[0x85] = "F22";
	keyMap[0x86] = "F23";
	keyMap[0x87] = "F24";

	keyMap[0x90] = "Numlock";
	keyMap[0x91] = "ScrollLock";
	keyMap[0xA0] = "LeftShift";
	keyMap[0xA1] = "RightShift";
	keyMap[0xA2] = "LeftCtrl";
	keyMap[0xA3] = "RightCtrl";
	keyMap[0xA4] = "LeftMenu";
	keyMap[0xA5] = "RightMenu";
}

void WindowWin::Create(int width, int height, bool fullScreen) {

	screenSize = Vector2(width, height);

	inputDevice.Initialize(3);

	InitKeyMap();

	closed = false;

	active = true;

	/*
	char t[1024];

    GetConsoleTitleA( t, 500 );     // retrieve the text
    // of the title bar of the console window

    cout << "The title of the console window is:" << endl;
    cout << t << endl << endl;
	*/

	HWND hwndConsole = GetConsoleWindow();

	//HWND hwndConsole = FindWindowA( NULL, t );  // FindWindowA actually
    // can get you the HWND of a window, based
    // on the text in its title bar!
    
    // Let's check to see it worked.  If the console window
    // moves after the function call below, then we know
    // that hwndConsole is really a valid handle to the console
    // window!
	//ShowWindow(hwndConsole,SW_HIDE);
    //MoveWindow( hwndConsole, 20, 20, 500, 500, true );  // test hwnd

    #pragma endregion

    #pragma region _REGION_B - get HINSTANCE and create a window!
    ////////////////////////
    // Getting the HINSTANCE given the HWND
    //
    // Want the HINSTANCE of a window, but
    // you only have its HWND?
    //
    // Here's how you generally get the HINSTANCE
    // of the console, based off of the HWND of
    // the console.
    HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hwndConsole, GWL_HINSTANCE);
    
    ////////////////////
    // Now I'm going to create an ACTUAL WINDOW.
    //
    // Note that you always need a HINSTANCE
    // to create a window, which is why I just
    // got it in the line just above here.
    WNDCLASS wc = {0};
    wc.hbrBackground =(HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor( hInstance, IDC_ARROW );
    wc.hIcon = LoadIcon( hInstance, IDI_APPLICATION );
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = TEXT("Pocket");   // name of window class
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (! RegisterClass( &wc ) )
    {
        cout << "Problem with your WNDCLASS, foo." << endl;
        return;   // ERR, SO QUIT
    }
    
    // Create a real live window!
    // (see http://bobobobo.wordpress.com/2008/01/31/how-to-create-a-basic-window-in-c/
    // for more details on basic windows)

	RECT correctSize;
	correctSize.top =0;
	correctSize.left =0;
	correctSize.right = width;
	correctSize.bottom = height;

	//WS_OVERLAPPEDWINDOW
	DWORD style = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX);
	
	if (AdjustWindowRect(&correctSize, style, false)) {
		width = correctSize.right - correctSize.left;
		height = correctSize.bottom - correctSize.top;
	}

    window = CreateWindow(  TEXT("Pocket"),
                    TEXT("Pocket"),
                    style,
                    100, 100, width, height,
                    NULL,   // if you make this hwndConsole, then
                            // the console becomes this window's parent
                            // Then, this window wouldn't get an
                            // entry in the taskbar
                    NULL,
                   
                    hInstance, NULL );

	handleToClass[window] = this;

    ShowWindow( window, SW_SHOWNORMAL );    // ShowWindow() on msdn
    UpdateWindow( window );

	EnableOpenGL();
}

void WindowWin::Destroy() {
	DisableOpenGL();
	DestroyWindow( window );
}

static Vector2 Position(LPARAM lparam) {
	return Vector2((float)LOWORD(lparam), (float)HIWORD(lparam));
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	WindowWin* window = handleToClass[hwnd];

    switch( message )
    {
		case WM_KEYDOWN:
			{
				std::string* key = window->GetKey((int)wparam);
				if (key) window->inputDevice.SetButton(*key, true);
			}
			
			break;

		case WM_KEYUP:
			{
				std::string* key = window->GetKey((int)wparam);
				if (key) window->inputDevice.SetButton(*key, false);
			}
			break;

		case WM_ACTIVATE:
			if (window) {
				window->active = !(wparam == 0);

				if (!window->active) {
					window->inputDevice.ReleaseAllButtons();
				}
			}
			//cout << "Activated : " << wparam << "" << endl;
			return 0;
		case WM_DESTROY:    // killing the window
			PostQuitMessage( 0 );
			window->closed = true;
			return 0;
			break;
			/*
		case WM_CREATE:
			cout << "WINDOW SPEAKING TO YOU THROUGH THE CONSOLE." << endl;
			cout << "I'm alive!" << endl << endl;
			Beep( 40, 40 );
			return 0;
			break;

        case WM_PAINT:  // paint event
            {
                cout << "PAINT EVENT!  time to repaint!!" << endl;
                PAINTSTRUCT ps;
                //HDC hdc;
                BeginPaint( hwnd, &ps );
                EndPaint( hwnd, &ps );
                return 0;
            }
            break;

        case WM_LBUTTONDOWN:    // left clicking on the client area of the window (white part)
			//window->input->InvokeTouch(0, Position(lparam), false);
            return 0;
            break;

		case WM_LBUTTONUP:    // left clicking on the client area of the window (white part)
			//window->input->InvokeTouch(0, Position(lparam), true);
			return 0;
			break;



        case WM_NCLBUTTONDOWN:  // NONCLIENT area leftbutton down (click on "title bar" part of window)
            //cout << "AAAH!! YER GONNA MOVE ME SON.  CAREFUL SON." << endl;
            //return 0;     // this is an interesting one.
            // try UNCOMMENTING the return 0; statement here.
            // Notice that you can NO LONGER move or manipulate
            // the window by clicking on its "title bar"
            // if you return 0; from here.  The reason for that
            // is the window movement is actually handled by
            // DefWindowProc().  That's why its so important
            // to remember to pass events you don't handle to
            // DefWindowProc() -- if you don't then the Window
            // won't act in the "default" way you're so used to
            // other windows acting in (in fact, it won't even
            // show up properly)
            break;

		
        case WM_CHAR:   // character key



            cout << "WHY R U CHARRING ME WITH " << (char)wparam << " FOR SON" << endl;
            return 0;
            break;

        case WM_MOVE:   // moving the window
            cout << "WHY R U MOVIN' ME TO x=" << LOWORD(lparam) << " y=" << HIWORD(lparam) << " FOR SON" << endl;
            return 0;
            break;

        case WM_SIZE:
            cout << "WHY R YA SIZIN' ME TO SIZE width=" << LOWORD(lparam) << " height=" << HIWORD(lparam) << " FOR SON"<< endl;
            return 0;
            break;

			*/

       
    }
    
    return DefWindowProc( hwnd, message, wparam, lparam );
}

bool WindowWin::Update(InputManager& input) {
	
	Vector2 mousePosition;
	POINT p;
	if (GetCursorPos(&p))
	{
		if (ScreenToClient(window, &p))
		{
		}

		mousePosition.x = (float)p.x;
		mousePosition.y = (float)p.y;
	}
	
	inputDevice.SetTouchPosition(0, mousePosition);
	inputDevice.SetTouchPosition(1, mousePosition);
	inputDevice.SetTouchPosition(2, mousePosition);

	inputDevice.SetTouch(0, active && ((GetKeyState(VK_LBUTTON) & 0x80) != 0), mousePosition);
	inputDevice.SetTouch(1, active && ((GetKeyState(VK_RBUTTON) & 0x80) != 0), mousePosition);
	inputDevice.SetTouch(2, active && ((GetKeyState(VK_MBUTTON) & 0x80) != 0), mousePosition);
	
	MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
    {
        if (!GetMessage(&msg, NULL, 0, 0)) return TRUE;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

	inputDevice.Update(input);
	return !closed;
}

bool TestEGLError(HWND hWnd, char* pszLocation)
{
	/*
		eglGetError returns the last error that has happened using egl,
		not the status of the last called function. The user has to
		check after every single egl call or at least once every frame.
	*/
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
//#ifndef NO_GDI
		//TCHAR pszStr[256];
		//_stprintf(pszStr, _T("%s failed (%d).\n"), pszLocation, iErr);
		//MessageBox(hWnd, pszStr, _T("Error"), MB_OK|MB_ICONEXCLAMATION);
//#endif
		return false;
	}

	return true;
}



void WindowWin::EnableOpenGL()
{
	hDC = GetDC(window);

	eglDisplay	= 0;
	eglConfig	= 0;
	eglSurface	= 0;
	eglContext	= 0;
	eglWindow	= window;
	ui32Vbo = 0; // Vertex buffer object handle

	bool g_bDemoDone = false;

	/*
		Step 1 - Get the default display.
		EGL uses the concept of a "display" which in most environments
		corresponds to a single physical screen. Since we usually want
		to draw to the main screen or only have a single screen to begin
		with, we let EGL pick the default display.
		Querying other displays is platform specific.
	*/
	eglDisplay = eglGetDisplay((NativeDisplayType) hDC);

    if(eglDisplay == EGL_NO_DISPLAY)
         eglDisplay = eglGetDisplay((NativeDisplayType) EGL_DEFAULT_DISPLAY);
	/*
		Step 2 - Initialize EGL.
		EGL has to be initialized with the display obtained in the
		previous step. We cannot use other EGL functions except
		eglGetDisplay and eglGetError before eglInitialize has been
		called.
		If we're not interested in the EGL version number we can just
		pass NULL for the second and third parameters.
	*/
	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		//MessageBox(0, _T("eglInitialize() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		g_bDemoDone = false;
	}

	/*
		Step 3 - Specify the required configuration attributes.
		An EGL "configuration" describes the pixel format and type of
		surfaces that can be used for drawing.
		For now we just want to use a 16 bit RGB surface that is a
		Window surface, i.e. it will be visible on screen. The list
		has to contain key/value pairs, terminated with EGL_NONE.
	*/
	int i = 0;
	pi32ConfigAttribs[i++] = EGL_RED_SIZE;
	pi32ConfigAttribs[i++] = 5;
	pi32ConfigAttribs[i++] = EGL_GREEN_SIZE;
	pi32ConfigAttribs[i++] = 6;
	pi32ConfigAttribs[i++] = EGL_BLUE_SIZE;
	pi32ConfigAttribs[i++] = 5;
	pi32ConfigAttribs[i++] = EGL_ALPHA_SIZE;
	pi32ConfigAttribs[i++] = 0;
	pi32ConfigAttribs[i++] = EGL_SURFACE_TYPE;
	pi32ConfigAttribs[i++] = EGL_WINDOW_BIT;
	pi32ConfigAttribs[i++] = EGL_NONE;

	/*
		Step 4 - Find a config that matches all requirements.
		eglChooseConfig provides a list of all available configurations
		that meet or exceed the requirements given as the second
		argument. In most cases we just want the first config that meets
		all criteria, so we can limit the number of configs returned to 1.
	*/
	EGLint iConfigs;
	if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		//MessageBox(0, _T("eglChooseConfig() failed."), _T("Error"), MB_OK|MB_ICONEXCLAMATION);
		g_bDemoDone = false;
	}

	/*
		Step 5 - Create a surface to draw to.
		Use the config picked in the previous step and the native window
		handle when available to create a window surface. A window surface
		is one that will be visible on screen inside the native display (or
		fullscreen if there is no windowing system).
		Pixmaps and pbuffers are surfaces which only exist in off-screen
		memory.
	*/
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

    if(eglSurface == EGL_NO_SURFACE)
    {
        eglGetError(); // Clear error
        eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}

	if (!TestEGLError(eglWindow, "eglCreateWindowSurface"))
	{
		g_bDemoDone = false;
	}

	/*
		Step 6 - Create a context.
		EGL has to create a context for OpenGL ES. Our OpenGL ES resources
		like textures will only be valid inside this context
		(or shared contexts)
	*/
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
	if (!TestEGLError(eglWindow, "eglCreateContext"))
	{
		g_bDemoDone = false;
	}

	

	/*
		Step 7 - Bind the context to the current thread and use our
		window surface for drawing and reading.
		Contexts are bound to a thread. This means you don't have to
		worry about other threads and processes interfering with your
		OpenGL ES application.
		We need to specify a surface that will be the target of all
		subsequent drawing operations, and one that will be the source
		of read operations. They can be the same surface.
	*/
	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	if (!TestEGLError(eglWindow, "eglMakeCurrent"))
	{
		g_bDemoDone = false;
	}


	/*


	PIXELFORMATDESCRIPTOR pfd;
	int format;
	
	// get the device context (DC)
	*hDC = GetDC( hWnd );
	
	// set the pixel format for the DC
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( *hDC, &pfd );
	SetPixelFormat( *hDC, format, &pfd );
	
	// create and enable the render context (RC)
	*hRC = wglCreateContext( *hDC );
	wglMakeCurrent( *hDC, *hRC );

	*/

	glMapBuffer = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
	glUnmapBuffer = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
}

// Disable OpenGL

void WindowWin::DisableOpenGL()
{
	
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglTerminate(eglDisplay);

}
void WindowWin::PreRender() {
	//glLoadIdentity();
}

void WindowWin::PostRender() {

	eglSwapBuffers(eglDisplay, eglSurface);
	//SwapBuffers( hDC );
}
