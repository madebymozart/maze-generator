cmake_minimum_required(VERSION 3.0)
project(TiledMazeGenerator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        source/cmd/cmd.hxx
        source/generator/generator.cxx
        source/generator/generator.hxx
        source/json/json.hxx
        source/mapper/mapper.cxx
        source/mapper/mapper.hxx
        source/main.cxx source/globals.hxx)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(TiledMazeGenerator ${SOURCE_FILES})
target_link_libraries(TiledMazeGenerator PRIVATE Threads::Threads)
