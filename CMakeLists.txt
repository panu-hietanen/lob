cmake_minimum_required(VERSION 3.20)
project(lob C)
enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(lob
    src/main.c
    src/price_level.c
    src/map.c
)

target_include_directories(lob PRIVATE src)

add_executable(test_price_level
    tests/test_price_level.c
    src/price_level.c
)
add_test(NAME price_level COMMAND test_price_level)

target_include_directories(test_price_level PRIVATE src)


if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(lob PRIVATE /fsanitize=address)
    target_compile_options(test_price_level PRIVATE /fsanitize=address)
elseif(NOT MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(lob PRIVATE -fsanitize=address)
    target_link_options(lob PUBLIC -fsanitize=address)
    target_compile_options(test_price_level PRIVATE -fsanitize=address)
    target_link_options(test_price_level PUBLIC -fsanitize=address)
endif()