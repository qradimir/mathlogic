cmake_minimum_required(VERSION 3.3)
project(mathlogic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(UTIL parser.cpp parser.h expression.cpp expression.h)
set(PROBLEM_A pr_a.cpp ${UTIL})

add_executable(problemA ${PROBLEM_A})