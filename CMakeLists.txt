cmake_minimum_required(VERSION 3.30)
project(AoC_2024)

foreach (val RANGE 1 7)
    add_subdirectory(${val})
endforeach ()