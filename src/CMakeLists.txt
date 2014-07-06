cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

add_definitions(-DRAPPELZLIB_LIBRARY)

include(AddVersionInfo OPTIONAL RESULT_VARIABLE versioncmakefound)
if( versioncmakefound )
    add_git_version_info(${TARGET_NAME}_GitVersion SOURCE_FILES)
endif()

add_library(${TARGET_NAME} ${SOURCE_FILES})
add_dependencies(${TARGET_NAME} ${TARGET_NAME}_GitVersion)

target_link_libraries(${TARGET_NAME} uv ${OPENSSL_LIBRARIES})
if(WIN32)
    target_link_libraries(${TARGET_NAME} odbc32 dbghelp)
else()
    target_link_libraries(${TARGET_NAME} odbc)
endif()

install(TARGETS ${TARGET_NAME} DESTINATION ./)
