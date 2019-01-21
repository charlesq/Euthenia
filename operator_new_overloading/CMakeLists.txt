cmake_minimum_required(VERSION 2.6)
project(test)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
find_package(Threads)
if (FALSE AND "$ENV{BUILD}" STREQUAL "1")
add_library(test STATIC t.cpp)
else ()
add_executable(test t.cpp)
endif()
target_link_libraries(test ${CMAKE_THREAD_LIBS_INIT})


