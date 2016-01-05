cmake_minimum_required(VERSION 2.8.12)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

include(AddVersionInfo OPTIONAL RESULT_VARIABLE versioncmakefound)
if( versioncmakefound )
    add_git_version_info(${TARGET_NAME}_GitVersion SOURCE_FILES)
endif()

find_package(OpenSSL REQUIRED)
find_package(ODBC REQUIRED)
set(LIBS uv ${OPENSSL_LIBRARIES} z iconv ${ODBC_LIBRARY})
if(WIN32)
    list(APPEND LIBS dbghelp)
endif()

add_lib(${TARGET_NAME} "${SOURCE_FILES}" "${LIBS}")
target_include_directories(${TARGET_NAME}
  PUBLIC lib packets ${ODBC_INCLUDE_DIR}
  PRIVATE ${OPENSSL_INCLUDE_DIR}
)
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_compile_definitions(${TARGET_NAME} PRIVATE NOMINMAX)
endif()

if(BUILD_SHARED_LIBS)
  target_compile_definitions(${TARGET_NAME}
    PRIVATE BUILDING_LIBRZU
    INTERFACE USING_RZU_SHARED
  )
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    find_file(OPENSSL_LIBEAY32_DLL "libeay32.dll" ${OPENSSL_INCLUDE_DIR}/../bin NO_DEFAULT_PATH)
    if(OPENSSL_LIBEAY32_DLL)
        install(PROGRAMS ${OPENSSL_LIBEAY32_DLL} DESTINATION ./ COMPONENT "binaries")
    endif()
endif()
