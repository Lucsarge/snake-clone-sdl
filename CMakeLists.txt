cmake_minimum_required(VERSION 3.15.0)

set(NAME snake-clone-sdl)

message(STATUS "using CMake generator: ${CMAKE_GENERATOR}")

project(${NAME})

#assumes the SDL was cloned at C:/SDL and produces the output in a new folder called SDL
add_subdirectory(C:/SDL ./SDL EXCLUDE_FROM_ALL)

add_executable(${NAME} snake-sdl/main.c)

target_link_libraries(${NAME} PRIVATE SDL3::SDL3)