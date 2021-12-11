cmake_minimum_required(VERSION 3.21)
project(blokus_board_game)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(blokus_board_game
        blokus.cpp
        blokus_original.cpp)
