cmake_minimum_required(VERSION 2.8)
project(RappelzLib)

set(CMAKE_VERBOSE_MAKEFILE false)
set(CMAKE_BUILD_TYPE "Debug")

set(SHARED_LIB_DIR "shared-lib")

get_filename_component(BUILD_DIR_NAME ${CMAKE_BINARY_DIR} NAME)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/${BUILD_DIR_NAME}-bin")
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/${BUILD_DIR_NAME}-bin")

set(SHARED_LIB_NAME "RappelzLib")

include_directories("${CMAKE_CURRENT_LIST_DIR}/${SHARED_LIB_DIR}")
include_directories("${CMAKE_CURRENT_LIST_DIR}/${SHARED_LIB_DIR}/Common")
include_directories("${CMAKE_CURRENT_LIST_DIR}/${SHARED_LIB_DIR}/Interfaces")
link_directories(${LIBRARY_OUTPUT_PATH})

if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO /INCREMENTAL:NO")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /MANIFEST:NO /INCREMENTAL:NO")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO /INCREMENTAL:NO")

  add_definitions("-D_CRT_SECURE_NO_WARNINGS")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

  if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libgcc -static-libstdc++ -Wl,--add-stdcall-alias")
  endif()

  #for qtcreator
  add_definitions("-D__GXX_EXPERIMENTAL_CXX0X__")
endif()

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath,.")
endif()

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/${SHARED_LIB_DIR}" ${SHARED_LIB_DIR})
