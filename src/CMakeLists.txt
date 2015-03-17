cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

include(AddVersionInfo OPTIONAL RESULT_VARIABLE versioncmakefound)
if( versioncmakefound )
    add_git_version_info(${TARGET_NAME}_GitVersion SOURCE_FILES)
endif()

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
set(LIBS uv ${OPENSSL_LIBRARIES})
if(WIN32)
    list(APPEND LIBS odbc32 dbghelp)
else()
    list(APPEND LIBS odbc)
endif()

add_lib(${TARGET_NAME} "${SOURCE_FILES}" "${LIBS}")
