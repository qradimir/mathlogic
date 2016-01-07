cmake_minimum_required(VERSION 3.3)
project(mathlogic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(UTIL
        expression.cpp expression.h
        parser.cpp parser.h
        expression_storage.cpp expression_storage.h)

set(PROBLEM_A pr_a.cpp ${UTIL})

add_executable(problemA ${PROBLEM_A})