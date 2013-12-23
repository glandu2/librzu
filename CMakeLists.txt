cmake_minimum_required(VERSION 2.8)
project(RappelzLib)

set(TARGET_NAME "RappelzLib")

add_definitions(-DUSING_UV_SHARED)
add_definitions(-DBUILDING_RAPPELZLIB)
include_directories("${CMAKE_CURRENT_LIST_DIR}/src")
include_directories("${CMAKE_CURRENT_LIST_DIR}/../libuv/include")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/src" src)
