cmake_minimum_required(VERSION 3.3)
project(mathlogic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(UTIL
        expression.cpp
        expression.h
        parser.cpp
        parser.h
        axioms.h
        proof.cpp
        proof.h)

add_executable(pr_a pr_a.cpp ${UTIL})
add_executable(tests test.cpp ${UTIL})