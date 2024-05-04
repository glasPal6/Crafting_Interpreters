cmake_minimum_required(VERSION 3.11) 
project(jlox)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_executable(${PROJECT_NAME} 
    src/main.c
)
# target_link_libraries(${PROJECT_NAME} 
# )
target_compile_options(${PROJECT_NAME} 
PRIVATE 
    -Werror 
    -Wall 
    -Wextra
)
# target_compile_options(${PROJECT_NAME} 
# PRIVATE 
#     -O3
#     -mtune=native
# )


