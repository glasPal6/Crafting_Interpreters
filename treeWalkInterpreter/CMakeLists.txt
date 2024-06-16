cmake_minimum_required(VERSION 3.11)
project(jlox)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#----------------------------------------------
# jlox build
#----------------------------------------------
add_executable(${PROJECT_NAME}
    src/main.c
)

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    LOGGING_IMPLEMENTATION
    TOKEN_IMPLENEMTATION
    TOKENLIST_IMPLEMENTATION
    SCANNER_IMPLEMENTATION
    EXPR_IMPLEMENTATION
)

target_compile_options(${PROJECT_NAME}
PRIVATE
    -Werror
    -Wall
    -Wextra
)

#----------------------------------------------
# expr util build
#----------------------------------------------
set(EXPR_UTIL expr_util)

add_executable(${EXPR_UTIL}
    utils/expr_util.c
)

target_compile_options(${EXPR_UTIL}
PRIVATE
    -Werror
    -Wall
    -Wextra
)
