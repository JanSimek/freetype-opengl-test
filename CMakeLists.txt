project(sdl2project)
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

find_package(OpenGL REQUIRED)
list(APPEND LIBRARIES ${OPENGL_LIBRARIES})
include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})

pkg_check_modules(GLEW glew)
list(APPEND LIBRARIES ${GLEW_LIBRARIES})
include_directories(SYSTEM ${GLEW_INCLUDE_DIRS})

pkg_check_modules(SDL2 sdl2)
list(APPEND LIBRARIES ${SDL2_LIBRARIES})
include_directories(SYSTEM ${SDL2_INCLUDE_DIRS})

pkg_check_modules(FREETYPE REQUIRED freetype2)
list(APPEND LIBRARIES ${FREETYPE_LIBRARIES})
include_directories(SYSTEM ${FREETYPE_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wno-c++11-extensions")

set(SOURCES
    Font.cpp
    Font.h
    Game.cpp
    Game.h
    GLSLShader.cpp
    GLSLShader.h
    Utility.h
    Utility.cpp
    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES} /usr/local/lib/libSOIL.a)

