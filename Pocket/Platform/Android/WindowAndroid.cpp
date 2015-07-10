//
//  WindowAndroid.cpp
//  Android
//
//  Created by Jeppe Nielsen on 03/07/15.
//
//

#include <jni.h>
#include <errno.h>
#include <math.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "WindowAndroid.hpp"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))


#include "WindowAndroid.hpp"


struct engine {
    android_app* app;

	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;

	int32_t touchX;
	int32_t touchY;
};

static android_app* state;

static engine* currentEngine = 0;


using namespace Pocket;
using namespace std;

static WindowAndroid* staticWindow;






//********************************





/**
 * Initialize an EGL context for the current display.
 * TODO tidy this up, currently it's mostly Google example code
 */
int init_display(struct engine* engine) {

	// Setup OpenGL ES 2
	// http://stackoverflow.com/questions/11478957/how-do-i-create-an-opengl-es-2-context-in-a-native-activity

	const EGLint attribs[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, //important
			EGL_BLUE_SIZE, 8,
			EGL_GREEN_SIZE, 8,
			EGL_RED_SIZE, 8,
			EGL_NONE
	};

	EGLint attribList[] =
	{
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
	};

	EGLint w, h, dummy, format;
	EGLint numConfigs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	eglInitialize(display, 0, 0);

	/* Here, the application chooses the configuration it desires. In this
	 * sample, we have a very simplified selection process, where we pick
	 * the first EGLConfig that matches our criteria */
	eglChooseConfig(display, attribs, &config, 1, &numConfigs);

	/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
	 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
	 * As soon as we picked a EGLConfig, we can safely reconfigure the
	 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	ANativeWindow_setBuffersGeometry(engine->app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, engine->app->window, NULL);

	context = eglCreateContext(display, config, NULL, attribList);

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGI("Unable to eglMakeCurrent");
		return -1;
	}

	// Grab the width and height of the surface
	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	engine->display = display;
	engine->context = context;
	engine->surface = surface;
	engine->width = w;
	engine->height = h;

	// Initialize GL state.
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//glEnable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	//glViewport(0, 0, w, h);

	return 0;
}

/**
 * Just the current frame in the display.
 */
void draw_frame(struct engine* engine) {
	// No display.
	if (engine->display == NULL) {
		return;
	}

	glClearColor(255,0,0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(engine->display, engine->surface);
}

/**
 * Tear down the EGL context currently associated with the display.
 */
void terminate_display(struct engine* engine) {
	if (engine->display != EGL_NO_DISPLAY) {
		eglMakeCurrent(engine->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (engine->context != EGL_NO_CONTEXT) {
			eglDestroyContext(engine->display, engine->context);
		}
		if (engine->surface != EGL_NO_SURFACE) {
			eglDestroySurface(engine->display, engine->surface);
		}
		eglTerminate(engine->display);
	}
	engine->display = EGL_NO_DISPLAY;
	engine->context = EGL_NO_CONTEXT;
	engine->surface = EGL_NO_SURFACE;
}

/**
 * Process the next input event.
 */
int32_t handle_input(struct android_app* app, AInputEvent* event) {
	struct engine* engine = (struct engine*)app->userData;
    
    int numTouches = AMotionEvent_getPointerCount(event);
    
    int32_t iAction = AMotionEvent_getAction(event);
    unsigned int flags = iAction & AMOTION_EVENT_ACTION_MASK;
    
    int32_t iIndex = (iAction & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
    
    //LOGI("TOUCH: iIndex: %d", iIndex);
    
    if (flags == AMOTION_EVENT_ACTION_POINTER_DOWN || flags == AMOTION_EVENT_ACTION_DOWN) {
        int id = AMotionEvent_getPointerId(event, iIndex);
        int xPos = AMotionEvent_getX(event, iIndex);
        int yPos = AMotionEvent_getY(event, iIndex);
        //LOGI("DOWN: id: %d x:%d\ty:%d\n",id, xPos, yPos);
        staticWindow->inputDevice.SetTouch(id, true, staticWindow->ConvertMousePosition(xPos, yPos));

    } else if (flags == AMOTION_EVENT_ACTION_POINTER_UP || flags == AMOTION_EVENT_ACTION_UP) {
        int id = AMotionEvent_getPointerId(event, iIndex);
        int xPos = AMotionEvent_getX(event, iIndex);
        int yPos = AMotionEvent_getY(event, iIndex);
       // LOGI("UP: id: %d x:%d\ty:%d\n",id, xPos, yPos);
        staticWindow->inputDevice.SetTouch(id, false, staticWindow->ConvertMousePosition(xPos, yPos));
    }
    
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        for (int i=0; i<numTouches; i++) {
            int id = AMotionEvent_getPointerId(event, i);
            int xPos = AMotionEvent_getX(event, i);
            int yPos = AMotionEvent_getY(event, i);
            //LOGI("move: x %d\ty %d\n",engine->touchX,engine->touchY);
            //LOGI("MOVE: id: %d x:%d\ty:%d\n",id, xPos, yPos);
            staticWindow->inputDevice.SetTouchPosition(id, staticWindow->ConvertMousePosition(xPos, yPos));
        }
    }
    
    /*
    
    for (int i=0; i<numTouches; i++) {
    
    LOGI("handle_input:: staticWindow = %i", (size_t)staticWindow);
	
    
    
    int32_t iAction = AMotionEvent_getAction(event);
    unsigned int flags = iAction & AMOTION_EVENT_ACTION_MASK;
    if (flags == AMOTION_EVENT_ACTION_DOWN)
    {
        int id = AMotionEvent_getPointerId(event, i);
        int xPos = AMotionEvent_getX(event, i);
        int yPos = AMotionEvent_getY(event, i);
        LOGI("DOWN: id: %d x:%d\ty:%d\n",id, xPos, yPos);
        staticWindow->inputDevice.SetTouch(id, true, staticWindow->ConvertMousePosition(xPos, yPos));
    } else if (flags == AMOTION_EVENT_ACTION_UP) {
        int id = AMotionEvent_getPointerId(event, i);
        int xPos = AMotionEvent_getX(event, i);
        int yPos = AMotionEvent_getY(event, i);
        LOGI("UP: id: %d x:%d\ty:%d\n",id, xPos, yPos);
        
        staticWindow->inputDevice.SetTouch(id, false, staticWindow->ConvertMousePosition(xPos, yPos));
        
    }
    
    }
    
    */
    
    //case AMOTION_EVENT_ACTION_UP:
    
    
	return 0;
}

/**
 * Process the next main command.
 */

void handle_cmd(struct android_app* app, int32_t cmd) {
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd) {
	case APP_CMD_SAVE_STATE:
		break;
	case APP_CMD_INIT_WINDOW:
		// The window is being shown, get it ready.
		if (engine->app->window != NULL) {
			init_display(engine);
            main();
            LOGI("main %i", (size_t)staticWindow);
		}
		break;
    case APP_CMD_GAINED_FOCUS:
        break;
	case APP_CMD_TERM_WINDOW:
		// The window is being hidden or closed, clean it up.
		terminate_display(engine);
		break;
	case APP_CMD_LOST_FOCUS:
		draw_frame(engine);
		break;
	}
}

void android_main(struct android_app* s) {
    app_dummy();
    
    staticWindow = 0;
    
    state = s;
    LOGI("WindowAndroid-1");
     engine engine;

	memset(&engine, 0, sizeof(engine));
	state->userData = &engine;
	state->onAppCmd = handle_cmd;
	state->onInputEvent = handle_input;
	engine.app = state;
    
    currentEngine = &engine;
    LOGI("WindowAndroid-4");
    
    main();
    
}

//***************************


void WindowAndroid::Create(int width, int height, bool fullScreen) {
    staticWindow = this;
    
    LOGI("WindowAndroid::Begin %i", (size_t)staticWindow);
    
    
    
 /*
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
    
    Window::Framebuffer = 0;//currentEngine->app->window;
     
    screenSize = Vector2(currentEngine->width, currentEngine->height);
    inputDevice.Initialize(11);
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
    //LOGI("width = %f, height = %f", screenSize.x, screenSize.y);
}

void WindowAndroid::PostRender() {
 
    eglSwapBuffers(currentEngine->display, currentEngine->surface);
}

Vector2 WindowAndroid::ConvertMousePosition(int x, int y) {
    return Vector2(x, screenSize.y - y);
}

void WindowAndroid::Begin() {
    // Read all pending events.
	while (1) {
		int ident;
		int events;
		struct android_poll_source* source;

		while ((ident=ALooper_pollAll(0, NULL, &events,(void**)&source)) >= 0) {

			// Process this event.
			if (source != NULL) {
				source->process(state, source);
			}

			// Check if we are exiting.
			if (state->destroyRequested != 0) {
				terminate_display(currentEngine);
				return;
			}
		}
        Step();
    }
}

void WindowAndroid::Loop() {
    Step();
}









