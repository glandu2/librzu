cmake_minimum_required(VERSION 2.8.12)

file(GLOB_RECURSE SOURCE_FILES *.cpp *.h)

add_rztest(${TARGET_NAME} "${SOURCE_FILES}" rzu)
