cmake_minimum_required(VERSION 2.8)
project(rzu)

set(TARGET_NAME rzu)

if(BUILD_SHARED_LIBS)
  add_definitions(-DUSING_UV_SHARED)
  add_definitions(-DBUILDING_LIBRZU)
endif()
include_directories("${CMAKE_CURRENT_LIST_DIR}/src")
include_directories("${CMAKE_CURRENT_LIST_DIR}/../libuv/include")

find_package(OpenSSL REQUIRED)

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/src" src)
