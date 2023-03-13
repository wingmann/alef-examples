cmake_minimum_required(VERSION 3.20)
project(alef_example VERSION 1.0.0 LANGUAGES CXX)

if (MSVC)
    set(CMAKE_CXX_FLAGS "/std:c++20")
else()
    set(CMAKE_CXX_FLAGS "-std=c++20 -Werror -Wall -Wextra -Wpedantic")
endif()

set(PROJECT_INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/lib/alef-core/include"
    "${CMAKE_SOURCE_DIR}/lib/julianah/include")

add_subdirectory(src)
