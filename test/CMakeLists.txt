cmake_minimum_required(VERSION 2.8)

if(BUILD_SHARED_LIBS)
  add_definitions(-DUSING_UV_SHARED)
  add_definitions(-DUSING_RZU_SHARED)
endif()
include_directories(${RZU_INCLUDE})

add_executable("${TARGET_NAME}_json_test" "JSONWriter.cpp")
target_link_libraries("${TARGET_NAME}_json_test" rzu)
add_test("${TARGET_NAME}_json_test" "${EXECUTABLE_OUTPUT_PATH}/${TARGET_NAME}_json_test")

add_executable("${TARGET_NAME}_perf_serialization" "test_perf.cpp")
target_link_libraries("${TARGET_NAME}_perf_serialization" rzu)
add_test("${TARGET_NAME}_perf_serialization" "${EXECUTABLE_OUTPUT_PATH}/${TARGET_NAME}_perf_serialization")
