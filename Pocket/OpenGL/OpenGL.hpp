#ifndef __gl_include_h__
#define __gl_include_h__

#include <iostream>

#ifdef IPHONE 

	#include <OpenGLES/ES3/gl.h>
    #include <OpenGLES/ES3/glext.h>

#define GL_WRITE_ONLY GL_WRITE_ONLY_OES
#define glMapBuffer glMapBufferOES
#define glUnmapBuffer glUnmapBufferOES

    #define OPENGL_EMBEDDED

    #define DISABLE_MAP_BUFFER

#elif WIN32

#define GL_GLEXT_PROTOTYPES
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>

#define GL_WRITE_ONLY GL_WRITE_ONLY_OES

extern PFNGLMAPBUFFEROESPROC glMapBuffer;
extern PFNGLUNMAPBUFFEROESPROC glUnmapBuffer;

	#define OPENGL_EMBEDDED

	#define DISABLE_MAP_BUFFER

#elif EMSCRIPTEN

    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>

    #define GL_WRITE_ONLY GL_WRITE_ONLY_OES
    #define glMapBuffer glMapBufferOES
    #define glUnmapBuffer glUnmapBufferOES

    #define OPENGL_EMBEDDED

    #define DISABLE_MAP_BUFFER

#elif ANDROID

    #include <EGL/egl.h>
    #include <GLES2/gl2.h>

#elif RASPBERRY_PI

    #include "GLES2/gl2.h"
    #include "GLES2/gl2ext.h"

#else

    #include <OpenGL/gl.h>
    #define OPENGL_DESKTOP

#define DISABLE_MAP_BUFFER

#endif

/*
void system_log(const char* format, ...)
{
    //va_list args;
    //char message[1024] = {0};
    //va_start(args, format);
    //vsnprintf(message, sizeof(message), format, args);

   // NSLog(@"%s", message);
   //std::cout << message << std::endl;
    //va_end(args);
}

*/


#ifdef WIN32

#define STATUS_CASE(enum) case enum: return #enum
static const char* _glStatusString(GLenum error)
{
	switch (error) {
		STATUS_CASE(GL_NO_ERROR);
		STATUS_CASE(GL_INVALID_ENUM);
		STATUS_CASE(GL_INVALID_VALUE);
		STATUS_CASE(GL_INVALID_OPERATION);
		STATUS_CASE(GL_INVALID_FRAMEBUFFER_OPERATION);
		STATUS_CASE(GL_OUT_OF_MEMORY);
		STATUS_CASE(GL_FRAMEBUFFER_COMPLETE);
		STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
		STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
		STATUS_CASE(GL_FRAMEBUFFER_UNSUPPORTED);
	}
	return 0;
}
#undef STATUS_CASE

#define ASSERT_GL(x)                                        \
            do {                                                \
                GLenum _glError;                                \
                x;                                              \
                _glError = glGetError();                        \
                if(_glError != GL_NO_ERROR) {                   \
std::cout<<__FILE__<<":"<<__LINE__<<" "<<#x<<" "<<_glStatusString(_glError)<<std::endl; \
                }                                               \
            } while(__LINE__ == -1)




#else

#define STATUS_CASE(enum) case enum: return #enum
static const char* _glStatusString(GLenum error)
{
    switch(error) {
        STATUS_CASE(GL_NO_ERROR);
        STATUS_CASE(GL_INVALID_ENUM);
        STATUS_CASE(GL_INVALID_VALUE);
        STATUS_CASE(GL_INVALID_OPERATION);
        STATUS_CASE(GL_INVALID_FRAMEBUFFER_OPERATION);
        STATUS_CASE(GL_OUT_OF_MEMORY);
        STATUS_CASE(GL_FRAMEBUFFER_COMPLETE);
        STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
#ifdef IPHONE
        STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS);
#endif
        STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
#ifndef EMSCRIPTEN
#ifndef ANDROID
#ifndef RASPBERRY_PI
        STATUS_CASE(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE);
#endif
#endif
#endif
        STATUS_CASE(GL_FRAMEBUFFER_UNSUPPORTED);
    }
    return 0;
}
#undef STATUS_CASE

    #define ASSERT_GL(x)                                        \
            do {                                                \
                GLenum _glError;                                \
                x;                                              \
                _glError = glGetError();                        \
                if(_glError != GL_NO_ERROR) {                   \
std::cout<<__FILE__<<":"<<__LINE__<<" "<<#x<<" "<<_glStatusString(_glError)<<std::endl; \
                }                                               \
            } while(__LINE__ == -1)


#endif
/*
system_log("%s:%d:  %s Error: %s\n",        \
                                __FILE__, __LINE__,             \
                                #x, _glStatusString(_glError)); \
*/


#endif
