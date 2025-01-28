
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("-- Found EMBER test '${name}'")

add_ember("${name}_assets" HEADER ARGS
		--file data.txt --name hello_world --prefix 0x48,0x65,0x6c,0x6c,0x6f,0x20 --suffix 0x21,0x00
)

# "CMake Style" pointers
set(my_header_file_var "${name}_assets_header")

add_executable(${name} main.c ${${my_header_file_var}})
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${name} PRIVATE "${name}_assets")
add_test(NAME ${name} COMMAND ${name})