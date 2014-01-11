cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

add_definitions(-DRAPPELZLIB_LIBRARY)

include(${CMAKE_CURRENT_LIST_DIR}/../../cmake/AddVersionInfo.cmake)
add_git_version_info(${TARGET_NAME}_GitVersion SOURCE_FILES)

add_library(${TARGET_NAME} ${SOURCE_FILES})
add_dependencies(${TARGET_NAME} ${TARGET_NAME}_GitVersion)

target_link_libraries(${TARGET_NAME} uv)

install(TARGETS ${TARGET_NAME} DESTINATION ./)
