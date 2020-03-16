cmake_minimum_required(VERSION 3.13)
project(c_exercise C)

set(CMAKE_C_STANDARD 11)

add_executable(c_magic_square magic_square.c)
add_executable(c_prime_exercise prime.c)
target_link_libraries(c_prime_exercise m)
add_custom_target(prime
        DEPENDS c_prime_exercise
        )
add_custom_command(OUTPUT prime_run
        COMMAND ./c_prime_exercise 3
        DEPENDS prime)
add_custom_target(run_prime
        DEPENDS prime_run
        )

add_custom_target(magic
        DEPENDS c_magic_square
        )
add_custom_command(OUTPUT magic_run
        COMMAND ./c_magic_square 3
        DEPENDS magic)
add_custom_target(run_magic
        DEPENDS magic_run
        )
