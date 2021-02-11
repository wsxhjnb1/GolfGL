#pragma once

#include <precomp.h>

// #define LOG(stream) stream << "[" << __FILE__ << ", " << __func__ << ", " << __LINE__ << "] "
#define BREAK_IF_FALSE(x) if (!(x)) __builtin_trap()
#define ASSERT(x, msg) do { if (!(x)) { LOG_FATAL("{}\n", msg); BREAK_IF_FALSE(false); } } while(0)
#define GLCALL(x) \
do{ clearAllOpenGlErrors(); x; BREAK_IF_FALSE(wasPreviousOpenGLCallSuccessful(__FILE__, __LINE__, #x)); } while (0)


void clearAllOpenGlErrors();
const char* openGLErrorToString(GLenum error);
bool wasPreviousOpenGLCallSuccessful(const char* file, int line, const char* call);

void clearAllOpenGlErrors() 
{
    do {} while (glGetError() != GL_NO_ERROR);
}


const char* openGLErrorToString(GLenum error) 
{
    switch(error) 
    {
        case GL_NO_ERROR: return "GL_NO_ERROR";
        case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
    }
        
    LOG_FATAL("Passed something that is not an error code");
    return "THIS_SHOULD_NEVER_HAPPEN";
}


bool wasPreviousOpenGLCallSuccessful(const char* file, int line, const char* call) 
{
    bool success = true;
    while (GLenum error = glGetError()) 
    {
        
        LOG_ERROR(  "[OpenGL error] {} {} \n\
                    File: {} \n\
                    Line: {} \n\
                    Call: {} \n\n"
                    , error, openGLErrorToString(error), file, line, call);

        success = false;
    }
    return success;
}

