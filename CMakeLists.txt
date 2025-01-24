cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(EMBER)

enable_testing()

file(GLOB_RECURSE EMBER_SOURCES "src/*.cpp")
add_executable(ember ${EMBER_SOURCES})

# Enable GNU warnings
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	target_compile_options(ember PRIVATE
			-Wall
			-Werror=return-type
			-Werror=return-local-addr
	)
endif()

# Enable MSVC warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

	# we4715 - treat missing return statement as error
	target_compile_options(ember PRIVATE /W4 /we4715)
endif()

# Function to be used by library users
function(add_ember target_name)
	cmake_parse_arguments(PARSE_ARGV 0 ARG
			"HEADER" "OUTPUT" "ARGS"
	)

	set(asset_store "${CMAKE_CURRENT_LIST_DIR}")

	# Default output location
	if (NOT ARG_OUTPUT)
		set(ARG_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${target_name}")
	endif()

	set(object_extension "o")
	set(object_flag "--elf")

	# handle windows
	if(CMAKE_SYSTEM MATCHES Windows)
		set(object_extension "obj")
		set(object_flag "--coff")
	endif()

	# Construct ember output filenames
	set(generated_object "${ARG_OUTPUT}.${object_extension}")

	if(ARG_HEADER)
		set(generated_header "${ARG_OUTPUT}.h")
		set(ADDITIONAL_FLAGS -H "${generated_header}")
		set("${target_name}_header" "${generated_header}" PARENT_SCOPE)
	endif()

	set(dependencies)

	# Extract dependencies
	foreach(i RANGE 0 ${ARG_ARGS_COUNT})
		list(GET ARG_ARGS ${i} element)

		if(element MATCHES "(--file)|(-f)")
			math(EXPR next_index "${i} + 1")
			list(GET ARG_ARGS ${next_index} next_element)

			# Convert to absolute path
			if(IS_ABSOLUTE "${next_element}")
				list(APPEND dependencies "${next_element}")
			else()
				list(APPEND dependencies "${asset_store}/${next_element}")
			endif()

		endif()
	endforeach()

	# Invoke ember to generate resource file
	add_custom_command(
			PRE_BUILD
			OUTPUT ${generated_object} ${generated_header}
			COMMAND ember ${ADDITIONAL_FLAGS} --store ${asset_store} ${object_flag} "${generated_object}" ${ARG_ARGS}
			DEPENDS ember ${dependencies}
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
			COMMENT "Generating asset file ${generated_object}"
	)

	# Create a library with our custom objects
	add_library(${target_name} OBJECT IMPORTED GLOBAL)

	# Some extra configuration needed
	set_target_properties(${target_name} PROPERTIES
			LINKER_LANGUAGE C
			EXCLUDE_FROM_ALL TRUE
			IMPORTED_OBJECTS ${generated_object}
	)
endfunction()

# Include test projects
if(EXISTS "${CMAKE_SOURCE_DIR}/test")
	FILE(GLOB tests RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/test ${CMAKE_CURRENT_SOURCE_DIR}/test/*)

	foreach(test ${tests})
		if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test/${test})
			add_subdirectory(test/${test})
		endif()
	endforeach()
endif()