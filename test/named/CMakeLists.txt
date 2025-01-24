
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("-- Found EMBER test '${name}'")

add_ember("${name}_assets" ARGS
		--file one.txt --name long_test_one
		--file two.txt --name long_test_two
)

add_executable(${name} main.c)
target_link_libraries(${name} PRIVATE "${name}_assets")
add_test(NAME ${name} COMMAND ${name})