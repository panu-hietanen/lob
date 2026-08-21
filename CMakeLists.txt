cmake_minimum_required(VERSION 3.20)
project(lob C)
enable_testing()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(lob
    src/main.c
    src/price_level.c
    src/map.c
    src/pool.c
    src/price_tree.c
)
target_include_directories(lob PRIVATE src)

add_executable(test_price_level
    tests/test_price_level.c
    src/price_level.c
)
add_test(NAME price_level COMMAND test_price_level)
target_include_directories(test_price_level PRIVATE src)

add_executable(test_map
    tests/test_map.c
    src/map.c
)
add_test(NAME map COMMAND test_map)
target_include_directories(test_map PRIVATE src)

add_executable(test_pool
    tests/test_pool.c
    src/pool.c
)
add_test(NAME pool COMMAND test_pool)
target_include_directories(test_pool PRIVATE src)

add_executable(test_tree
    tests/test_tree.c
    src/price_tree.c
)
add_test(NAME tree COMMAND test_tree)
target_include_directories(test_tree PRIVATE src)


if (MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(lob PRIVATE /fsanitize=address)
    target_compile_options(test_price_level PRIVATE /fsanitize=address)
    target_compile_options(test_map PRIVATE /fsanitize=address)
    target_compile_options(test_pool PRIVATE /fsanitize=address)
    target_compile_options(test_tree PRIVATE /fsanitize=address)
elseif(NOT MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(lob PRIVATE -fsanitize=address)
    target_link_options(lob PUBLIC -fsanitize=address)
    target_compile_options(test_price_level PRIVATE -fsanitize=address)
    target_link_options(test_price_level PUBLIC -fsanitize=address)
    target_compile_options(test_map PRIVATE -fsanitize=address)
    target_link_options(test_map PUBLIC -fsanitize=address)
    target_compile_options(test_pool PRIVATE -fsanitize=address)
    target_link_options(test_pool PUBLIC -fsanitize=address)
    target_compile_options(test_tree PRIVATE -fsanitize=address)
    target_link_options(test_tree PUBLIC -fsanitize=address)
endif()