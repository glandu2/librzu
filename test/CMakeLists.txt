cmake_minimum_required(VERSION 2.8.12)

add_executable("${TARGET_NAME}_json_test" "JSONWriter.cpp")
target_link_libraries("${TARGET_NAME}_json_test" rzu)
add_test(NAME "${TARGET_NAME}_json_test" COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}_json_test" WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable("${TARGET_NAME}_perf_serialization" "test_perf.cpp")
target_link_libraries("${TARGET_NAME}_perf_serialization" rzu)
add_test(NAME "${TARGET_NAME}_perf_serialization" COMMAND "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}_perf_serialization" WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
