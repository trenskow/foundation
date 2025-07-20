cmake_minimum_required(VERSION 3.22)

project(foundation DESCRIPTION "Foundation is my own \"standard library\" for C++.")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Wno-vla-extension -std=c++20 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(NOT LIBRARY_BUILD_TYPE)
	set(LIBRARY_BUILD_TYPE SHARED)
endif()

add_library(foundation ${LIBRARY_BUILD_TYPE}
	src/exceptions/exception.cpp
	src/memory/allocator.cpp
	src/memory/object.cpp
	src/parallelism/mutex.cpp
	src/parallelism/thread.cpp
	src/serialization/json.cpp
	src/system/endian.cpp
	src/system/environment.cpp
	src/tools/regular-expression.cpp
	src/types/date.cpp
	src/types/duration.cpp
	src/types/hashable.cpp
	src/types/null.cpp
	src/types/string.cpp
	src/types/type.cpp
	src/types/unicode.cpp
	src/types/url.cpp
	src/types/uuid.cpp)

if(FOUNDATION_IO_FS)
	message(STATUS "Foundation: namespace `io::fs` enabled")
	add_compile_definitions(FOUNDATION_IO_FS)
	target_sources(foundation PRIVATE
		src/io/fs/file.cpp)
endif()

set(ARCH_OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_PROCESSOR}")

set_target_properties(foundation
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${ARCH_OUTPUT_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${ARCH_OUTPUT_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${ARCH_OUTPUT_DIR}/bin")

target_compile_features(foundation PRIVATE cxx_std_20)
