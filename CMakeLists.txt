cmake_minimum_required(VERSION 3.20)
project(ArcKnight VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Force high-optimization and native CPU instruction sets
if(MSVC)
    add_compile_options(/O2 /Oi /W4 /std:c++20)
else()
    add_compile_options(-O3 -Wall -Wextra -march=native -std=c++20)
endif()

add_subdirectory(engine)
add_subdirectory(tests)