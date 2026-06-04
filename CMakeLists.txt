cmake_minimum_required(VERSION 3.10)
project(3DSDL_exec C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

# 1. Compile your rendering/physics code into a reusable static library (.a / .lib)
add_library(3DSDL STATIC 
    src/renderer.c
)

target_include_directories(3DSDL PUBLIC include)

# Link SDL dependencies directly to your library
target_link_libraries(3DSDL 
    SDL2::SDL2 
    SDL2_ttf::SDL2_ttf
    m
)

# 2. OPTIONAL: Keep main.c as an executable to test your library locally
add_executable(3DSDL_exec src/main.c)

# Link your fresh library directly into your test app
target_link_libraries(3DSDL_exec 
	3DSDL
)
