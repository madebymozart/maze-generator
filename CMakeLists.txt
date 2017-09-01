cmake_minimum_required(VERSION 3.0)
project(TMXMazeGenerator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        source/cmd/cmd.hxx
        source/generator/generator.cxx
        source/generator/generator.hxx
        source/json/json.hxx
        source/mapper/mapper.cxx
        source/mapper/mapper.hxx
        source/main.cxx source/globals.hxx)

add_executable(TMXMazeGenerator ${SOURCE_FILES})