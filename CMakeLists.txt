cmake_minimum_required(VERSION 3.13)
project(c_exercise C)

set(CMAKE_C_STANDARD 11)

add_executable(prime prime.c)
target_link_libraries(prime m)
add_custom_target(run_prime
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/prime 3
        DEPENDS prime
        )

add_executable(magic magic_square.c)
add_custom_target(run_magic
        COMMAND "${CMAKE_CURRENT_BINARY_DIR}/magic" 3
        DEPENDS magic)
