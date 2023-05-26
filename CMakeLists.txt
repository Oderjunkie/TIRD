cmake_minimum_required (VERSION 3.0.0)
project (main C)

SET(CMAKE_EXPORT_COMPILE_COMMANDS 1)
SET(CMAKE_C_COMPILER "gcc")
SET(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES})
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -O0")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lreadline")
add_compile_options(-pedantic -pedantic-errors -Wall -Wextra -Werror -lreadline)

include_directories(include)
file(GLOB_RECURSE SRC src/*.c)
add_executable(main ${SRC})

add_custom_target(
	copy-compile-commands ALL
	${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_BINARY_DIR}/compile_commands.json
	${CMAKE_CURRENT_LIST_DIR}
)
