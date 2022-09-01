cmake_minimum_required(VERSION 3.23)
project(sydney)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
add_subdirectory(tests)

add_library(sydney
        lib/include/sydney/awaitable_future.h
        lib/include/sydney/coroutine.h
        lib/include/sydney/local_scheduler.h
        lib/include/sydney/scheduler.h
        lib/include/sydney/sydney.h
        lib/include/sydney/thread_pool.h

        lib/src/local_scheduler.cpp
        lib/src/scheduler.cpp
        lib/src/thread_pool.cpp lib/include/sydney/pollable.h lib/src/pollable.cpp)

target_include_directories(sydney PUBLIC lib/include)
