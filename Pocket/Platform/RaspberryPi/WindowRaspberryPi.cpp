//
//  WindowRaspberryPi.cpp
//  
//
//  Created by Jeppe Nielsen on 3/31/14.
//  Copyright (c) 2018 Jeppe Nielsen. All rights reserved.
//

#include "WindowRaspberryPi.hpp"
#include <iostream>
#include <assert.h>
#include "bcm_host.h"

#include "OpenGL.hpp"
#include "EGL/egl.h"
#include "EGL/eglext.h"

using namespace Pocket;
using namespace std;

typedef struct {
   uint32_t screen_width;
   uint32_t screen_height;
   DISPMANX_DISPLAY_HANDLE_T dispman_display;
   DISPMANX_ELEMENT_HANDLE_T dispman_element;
   EGLDisplay display;
   EGLSurface surface;
   EGLContext context;
} CUBE_STATE_T;

static void init_ogl(CUBE_STATE_T *state);

static CUBE_STATE_T _state, *state=&_state;

static void init_ogl(CUBE_STATE_T *state) {
   int32_t success = 0;
   EGLBoolean result;
   EGLint num_config;

   static EGL_DISPMANX_WINDOW_T nativewindow;

   DISPMANX_ELEMENT_HANDLE_T dispman_element;
   DISPMANX_DISPLAY_HANDLE_T dispman_display;
   DISPMANX_UPDATE_HANDLE_T dispman_update;
   VC_RECT_T dst_rect;
   VC_RECT_T src_rect;

    static const EGLint attribute_list[] = {
        EGL_RED_SIZE,       8,
        EGL_GREEN_SIZE,     8,
        EGL_BLUE_SIZE,      8,
        EGL_ALPHA_SIZE,     8,
        EGL_DEPTH_SIZE,     24,
        EGL_STENCIL_SIZE,   8,
        EGL_SAMPLE_BUFFERS, 0,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };
   
    static const EGLint context_attributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

   EGLConfig config;

   state->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
   assert(state->display!=EGL_NO_DISPLAY);
   
   result = eglInitialize(state->display, NULL, NULL);
   assert(EGL_FALSE != result);
   
   result = eglChooseConfig(state->display, attribute_list, &config, 1, &num_config);
   assert(EGL_FALSE != result);
   
   result = eglBindAPI(EGL_OPENGL_ES_API);
   assert(EGL_FALSE != result);
   
   state->context = eglCreateContext(state->display, config, EGL_NO_CONTEXT, context_attributes);
   assert(state->context!=EGL_NO_CONTEXT);
   
   success = graphics_get_display_size(0 /* LCD */, &state->screen_width, &state->screen_height);
   assert( success >= 0 );

   dst_rect.x = 0;
   dst_rect.y = 0;
   dst_rect.width = state->screen_width;
   dst_rect.height = state->screen_height;
    
   src_rect.x = 0;
   src_rect.y = 0;
   src_rect.width = state->screen_width << 16;
   src_rect.height = state->screen_height << 16;

   dispman_display = vc_dispmanx_display_open(0);
   dispman_update = vc_dispmanx_update_start(0);
    
   dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display,
      0, &dst_rect, 0,
      &src_rect, DISPMANX_PROTECTION_NONE, 0, 0, (DISPMANX_TRANSFORM_T)0);
    
   nativewindow.element = dispman_element;
   nativewindow.width = state->screen_width;
   nativewindow.height = state->screen_height;
   vc_dispmanx_update_submit_sync( dispman_update );
    
   state->surface = eglCreateWindowSurface( state->display, config, &nativewindow, NULL );
   assert(state->surface != EGL_NO_SURFACE);

   result = eglMakeCurrent(state->display, state->surface, state->surface, state->context);
   assert(EGL_FALSE != result);
}

void WindowRaspberryPi::Create(int width, int height, bool fullScreen) {
    bcm_host_init();
    memset( state, 0, sizeof( *state ) );
    init_ogl(state);
    context->ScreenSize = Vector2(state->screen_width, state->screen_height);
}

void WindowRaspberryPi::Destroy() {

}

bool WindowRaspberryPi::Update() {
    
    return true;
}

void WindowRaspberryPi::PreRender() {
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    const Vector2& screenSize = context->ScreenSize;
    ASSERT_GL(glViewport(0, 0, (GLsizei)screenSize.x, (GLsizei)screenSize.y));
    ASSERT_GL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    ASSERT_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    ASSERT_GL(glEnable(GL_CULL_FACE));
}

void WindowRaspberryPi::PostRender() {
    eglSwapBuffers(state->display, state->surface);
}

Vector2 WindowRaspberryPi::ConvertMousePosition(int x, int y) {
    return Vector2(x, y);
}

int WindowRaspberryPi::GetMouseButtonIndex(int index) {
    return 1;
}

InputDevice& WindowRaspberryPi::inputDevice() {
    return context->InputDevice();
}
