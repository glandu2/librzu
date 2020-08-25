cmake_minimum_required(VERSION 2.8.12)

option(RZU_ENABLE_PACKET_PREPROCESS_TARGETS
	"If enabled, one target for each packet will be generated to generate the C++ struct of that packet"
	OFF
)

if(RZU_ENABLE_PACKET_PREPROCESS_TARGETS)
	find_program(CLANG_FORMAT_EXECUTABLE
				 NAMES clang-format
					   clang-format-7
					   clang-format-6.0
					   clang-format-5.0
					   clang-format-4.0
					   clang-format-3.9
					   clang-format-3.8
					   clang-format-3.7
					   clang-format-3.6
					   clang-format-3.5
					   clang-format-3.4
					   clang-format-3.3
				 DOC "clang-format executable")
	if(CLANG_FORMAT_EXECUTABLE)
		file(GLOB packet_files RELATIVE ${CMAKE_CURRENT_LIST_DIR}/../packets/GameClient ${CMAKE_CURRENT_LIST_DIR}/../packets/GameClient/*.h)
		foreach(packet_file ${packet_files})
			add_custom_target(preprocess_${packet_file}
				COMMAND ${CMAKE_CXX_COMPILER} -E -DDEBUG_PREPROCESSOR  -I../../lib ${packet_file} | $<SHELL_PATH:${CLANG_FORMAT_EXECUTABLE}> -style llvm -
				WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../packets/GameClient
				COMMENT "Preprocessing packet ${packet_file}"
				SOURCES ${CMAKE_CURRENT_LIST_DIR}/../packets/GameClient/${packet_file})
		endforeach()
	endif()
endif()
