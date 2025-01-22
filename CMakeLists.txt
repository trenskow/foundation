cmake_minimum_required(VERSION 3.22)

project(shared-foundation-cpp DESCRIPTION "Shared C++ library for 0-bit Games.")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wno-vla-extension")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(NOT LIBRARY_BUILD_TYPE)
	set(LIBRARY_BUILD_TYPE SHARED)
endif()

add_library(shared-foundation-cpp ${LIBRARY_BUILD_TYPE}
	)

set(ARCH_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_PROCESSOR}")

set_target_properties(shared-foundation-cpp
	PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY "${ARCH_OUTPUT_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${ARCH_OUTPUT_DIR}/bin")

target_compile_features(shared-foundation-cpp PRIVATE cxx_std_17)
