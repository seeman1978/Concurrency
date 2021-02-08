cmake_minimum_required(VERSION 3.17)
project(Concurrency)

set(CMAKE_CXX_STANDARD 20)

add_executable(Concurrency threadsafe_queue2.cpp threadsafe_queue3.cpp)