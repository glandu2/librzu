cmake_minimum_required(VERSION 2.8)
project(rzu)

set(TARGET_NAME rzu)

if(BUILD_SHARED_LIBS)
  add_definitions(-DUSING_UV_SHARED)
  add_definitions(-DBUILDING_LIBRZU)
endif()

set(RZU_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src" CACHE STRING "librzu include directory")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src")
include_directories("${UV_INCLUDE}")
include_directories("${Z_INCLUDE}")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/src" src)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test" test)
