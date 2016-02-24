cmake_minimum_required(VERSION 3.3)
project(mathlogic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(UTIL
        expression.cpp
        expression.h
        expression_scheme.cpp
        expression_scheme.h
        parser.cpp
        parser.h
        axioms.cpp
        axioms.h
        proof.cpp
        proof.h)

add_executable(pr_a pr_a.cpp ${UTIL})
add_executable(pr_b pr_b.cpp ${UTIL})
add_executable(pr_c pr_c.cpp ${UTIL})