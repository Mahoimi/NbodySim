cmake_minimum_required(VERSION 2.6)
project(AntTweakBar)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

file(
    GLOB_RECURSE
    ANTTWEAKBAR_HEADERS
    src/*.h include/*.h
)

set(
  ANTTWEAKBAR_SOURCES 
  src/TwColors.cpp 
  src/TwFonts.cpp 
  src/TwOpenGL.cpp 
  src/TwOpenGLCore.cpp 
  src/TwBar.cpp 
  src/TwMgr.cpp 
  src/TwPrecomp.cpp 
  src/LoadOGL.cpp 
  src/LoadOGLCore.cpp 
  src/TwEventGLFW.c 
  src/TwEventGLUT.c 
  src/TwEventSDL.c 
  src/TwEventSDL12.c 
  src/TwEventSDL13.c 
  src/TwEventSFML.cpp 
  src/TwEventX11.c
)

add_library(AntTweakBar STATIC ${ANTTWEAKBAR_SOURCES} ${ANTTWEAKBAR_HEADERS})

if(WIN32)
  set_target_properties(AntTweakBar PROPERTIES COMPILE_FLAGS "-D_WINDOWS")
endif()

if(UNIX)
  set_target_properties(AntTweakBar PROPERTIES COMPILE_FLAGS "-D_UNIX")
endif()
