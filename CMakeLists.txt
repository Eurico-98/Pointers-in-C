cmake_minimum_required(VERSION 3.15)
project(Projecto C)

set(CMAKE_C_STANDARD 90)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-Wall -Wextra")

add_executable(Projecto main.c)