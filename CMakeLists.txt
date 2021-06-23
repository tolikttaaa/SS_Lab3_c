cmake_minimum_required(VERSION 3.19)
project(SS_Lab3_c C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Wextra -pedantic -Werror)

add_library(
        ListModeLib
        SHARED
        utils.c list_mode.c fat32_lib.c ListModeLib.c
)

add_library(
        ScriptModeLib
        SHARED
        utils.c script_mode.c fat32_lib.c ScriptModeLib.c
)