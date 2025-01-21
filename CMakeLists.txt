cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(EMBER)

file(GLOB_RECURSE EMBER_SOURCES "src/*.cpp")
add_executable(main ${EMBER_SOURCES})

# Enable GNU warnings
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(main PRIVATE
			-Wall
			-Werror=return-type
			-Werror=return-local-addr
	)
endif()

# Enable MSVC warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

	# we4715 - treat missing return statement as error
	target_compile_options(main PRIVATE /W4 /we4715)
endif()
