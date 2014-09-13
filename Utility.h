#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>      // std::pair

#include <SDL2/SDL_opengl.h>

#define ARRAY_LEN(x)        (sizeof(x) / sizeof(*(x)))

std::string GLErrorString( GLenum errorCode );
#define GL_CHECK_ERROR() \
    do\
    { \
        int error = glGetError(); \
        if (error != GL_NO_ERROR) \
        { \
            std::cerr << "OpenGL error in " << __FILE__ << ":" << __LINE__ << " / " << __FUNCTION__ << ": " \
                      << GLErrorString(error) << std::endl; \
            exit(1); \
        } \
    } while(0)

#endif // UTILITY_H
