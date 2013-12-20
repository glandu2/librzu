cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE FILE_SOURCES *.cpp *.h)

add_definitions(-DRAPPELZLIB_LIBRARY)

add_library(${TARGET_NAME} ${FILE_SOURCES})
target_link_libraries(${TARGET_NAME} uv)

install(TARGETS ${TARGET_NAME} DESTINATION ./)
