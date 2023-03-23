cmake_minimum_required(VERSION 3.20)
project(alef_example VERSION 1.0.0 LANGUAGES CXX)

if (MSVC)
    set(CMAKE_CXX_FLAGS "/std:c++20")
else()
    set(CMAKE_CXX_FLAGS "-std=c++20 -Werror -Wall -Wextra -Wpedantic")
endif()

set(ALEF_INCLUDE_PATH
    "${CMAKE_SOURCE_DIR}/lib/alef/include"
    "${CMAKE_SOURCE_DIR}/lib/biginteger/include"
    "${CMAKE_SOURCE_DIR}/lib/cast/include"
    "${CMAKE_SOURCE_DIR}/lib/concepts/include"
    "${CMAKE_SOURCE_DIR}/lib/containers/include"
    "${CMAKE_SOURCE_DIR}/lib/err-fusion/include"
    "${CMAKE_SOURCE_DIR}/lib/iterators/include"
    "${CMAKE_SOURCE_DIR}/lib/julianah/include"
    "${CMAKE_SOURCE_DIR}/include")

set(PROJECT_INCLUDE_DIRS ${ALEF_INCLUDE_PATH})

add_subdirectory(src)
