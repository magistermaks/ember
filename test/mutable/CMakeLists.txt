
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("-- Found EMBER test '${name}'")

add_ember("${name}_assets" MUTABLE HEADER ARGS
		--file data.txt --name mdat
)

# "CMake Style" pointers
set(my_header_file_var "${name}_assets_header")

add_executable(${name} main.c ${${my_header_file_var}})
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${name} PRIVATE "${name}_assets")
add_test(NAME ${name} COMMAND ${name})