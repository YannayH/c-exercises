cmake_minimum_required(VERSION 3.15)
project(c_exercise C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-Wl,--wrap,malloc -Wl,--wrap,free")

add_executable(prime prime.c)
target_link_libraries(prime m)
add_custom_target(run_prime
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/prime 3
        DEPENDS prime
        )

add_executable(magic magic_square.c)
add_custom_target(run_magic
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/magic 5
        DEPENDS magic)

add_executable(merge_pointers_a merge_pointers_a.c)
add_custom_target(run_merge_pointers_a
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/merge_pointers_a
        DEPENDS merge_pointers_a)

add_executable(merge_pointers_b merge_pointers_b.c)
add_custom_target(run_merge_pointers_b
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/merge_pointers_b abc av
        DEPENDS merge_pointers_b)
add_custom_target(valgrind_merge_pointers_b
        COMMAND valgrind ${CMAKE_CURRENT_BINARY_DIR}/merge_pointers_b abc av
        DEPENDS merge_pointers_b)

add_library(memmanager SHARED memmanager.c)
target_link_libraries(magic memmanager)


add_executable(mem_user memuser.c)
target_link_libraries(mem_user memmanager)
add_custom_target(run_mem_user
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/mem_user
        DEPENDS mem_user)
add_custom_target(valgrind_mem_user
        COMMAND valgrind ${CMAKE_CURRENT_BINARY_DIR}/mem_user
        DEPENDS mem_user)