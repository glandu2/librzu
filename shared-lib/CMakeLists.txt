cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE FILE_SOURCES *.cpp *.h)
set(TARGET_NAME ${SHARED_LIB_NAME})

add_definitions(-DRAPPELZLIB_LIBRARY)

add_library(${TARGET_NAME} SHARED ${FILE_SOURCES})
if(WIN32)
	target_link_libraries(${TARGET_NAME} D:/Programmes/Programmation/libraries/OpenSSL/lib/libeay32.lib)
	include_directories(D:/Programmes/Programmation/libraries/OpenSSL/include)
else()
target_link_libraries(${TARGET_NAME} crypto)
endif()

install(TARGETS ${TARGET_NAME} DESTINATION ./)
