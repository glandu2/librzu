cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

include(AddVersionInfo OPTIONAL RESULT_VARIABLE versioncmakefound)
if( versioncmakefound )
    add_git_version_info(${TARGET_NAME}_GitVersion SOURCE_FILES)
endif()

if(BUILD_SHARED_LIBS)
  add_definitions(-DUSING_UV_SHARED)
  add_definitions(-DBUILDING_LIBRZU)
endif()

find_package(OpenSSL REQUIRED)
find_package(ODBC REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR} ${ODBC_INCLUDE_DIR})
set(LIBS uv ${OPENSSL_LIBRARIES} z iconv)
list(APPEND LIBS ${ODBC_LIBRARY})
if(WIN32)
    list(APPEND LIBS dbghelp)
endif()

add_lib(${TARGET_NAME} "${SOURCE_FILES}" "${LIBS}")

if(DETECTED_OS_WIN)
    find_file(OPENSSL_LIBEAY32_DLL "libeay32.dll" ${OPENSSL_INCLUDE_DIR}/../bin NO_DEFAULT_PATH)
    if(OPENSSL_LIBEAY32_DLL)
        install(PROGRAMS ${OPENSSL_LIBEAY32_DLL} DESTINATION ./ COMPONENT "binaries")
    endif()
endif()
