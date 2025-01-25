
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("-- Found EMBER test '${name}'")

add_ember("${name}_assets" HEADER ARGS
		--file test_a.txt --limit 10   --name test_a_10
		--file test_b.txt --limit 0x10 --name test_b_16
		--file test_a.txt --limit 0b10 --name test_a_2
		--file test_b.txt --limit 0o10 --name test_b_8
)

# "CMake Style" pointers
set(my_header_file_var "${name}_assets_header")

add_executable(${name} main.c ${${my_header_file_var}})
target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${name} PRIVATE "${name}_assets")
add_test(NAME ${name} COMMAND ${name})