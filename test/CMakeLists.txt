cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

set(LIBS rzu)
add_rztest(${TARGET_NAME} "${SOURCE_FILES}" "${LIBS}")
