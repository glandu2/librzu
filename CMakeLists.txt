cmake_minimum_required(VERSION 2.8)
project(RappelzLib)

set(TARGET_NAME "RappelzLib")

include_directories("${CMAKE_CURRENT_LIST_DIR}/src")
include_directories("${CMAKE_CURRENT_LIST_DIR}/../libuv/include")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/src" src)
