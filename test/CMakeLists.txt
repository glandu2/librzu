cmake_minimum_required(VERSION 2.8.12)

add_executable("${TARGET_NAME}_json_test" "JSONWriter.cpp")
target_link_libraries("${TARGET_NAME}_json_test" rzu)
add_test("${TARGET_NAME}_json_test" "${EXECUTABLE_OUTPUT_PATH}/${TARGET_NAME}_json_test")

add_executable("${TARGET_NAME}_perf_serialization" "test_perf.cpp")
target_link_libraries("${TARGET_NAME}_perf_serialization" rzu)
add_test("${TARGET_NAME}_perf_serialization" "${EXECUTABLE_OUTPUT_PATH}/${TARGET_NAME}_perf_serialization")
