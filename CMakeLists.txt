cmake_minimum_required(VERSION 2.8)
project(rzu)

set(TARGET_NAME rzu)

set(RZU_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/src" CACHE STRING "librzu include directory")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src")
include_directories("${UV_INCLUDE}")
include_directories("${Z_INCLUDE}")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/src" src)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test" test)
