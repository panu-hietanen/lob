cmake_minimum_required(VERSION 3.20)
project(lob C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(lob
    src/main.c
)

target_include_directories(lob PRIVATE src)

if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(lob PRIVATE /fsanitize=address)
elseif(NOT MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(lob PRIVATE -fsanitize=address)
    target_link_options(lob PUBLIC -fsanitize=address)
endif()