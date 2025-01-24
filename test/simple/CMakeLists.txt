
get_filename_component(name ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("-- Found EMBER test '${name}'")

add_ember(hello_assets ARGS
	--file hello.txt
)

add_executable(${name} main.c)
target_link_libraries(${name} PRIVATE hello_assets)
add_test(NAME ${name} COMMAND ${name})