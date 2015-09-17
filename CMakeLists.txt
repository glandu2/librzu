cmake_minimum_required(VERSION 2.8)
project(rzu)

set(TARGET_NAME rzu)

set(RZU_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/src/lib ${CMAKE_CURRENT_LIST_DIR}/src/packets CACHE STRING "librzu include directory")
include_directories(${UV_INCLUDE})
include_directories(${Z_INCLUDE})
include_directories(${ICONV_INCLUDE})
include_directories(${RZU_INCLUDE})

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/src" src)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/test" test)
