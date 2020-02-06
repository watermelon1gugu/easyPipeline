cmake_minimum_required(VERSION 3.10)
project(easyPipeline)

set(CMAKE_CXX_STANDARD 14)

add_executable(easyPipeline main.cpp Context.h Context.cpp FuncItem.cpp FuncItem.h Pipeline.cpp Pipeline.h StreamPipeline.h)