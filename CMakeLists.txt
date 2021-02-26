cmake_minimum_required(VERSION 3.17)
project(Concurrency)

set(CMAKE_CXX_STANDARD 20)

add_executable(Concurrency threadsafe_queue2.cpp threadsafe_queue3.cpp threadsafe_queue4.cpp threadsafe_queue5.cpp spinlock_mutex.cpp lock_free_stack.cpp lock_free_stack2.cpp lock_free_queue.cpp)