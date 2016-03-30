cmake_minimum_required(VERSION 3.3)
project(mathlogic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(UTIL
        Propositional/expression.cpp
        Propositional/expression.h
        Propositional/parser.cpp
        Propositional/parser.h
        Propositional/axioms.cpp
        Propositional/axioms.h
        Propositional/proof.cpp
        Propositional/proof.h
        Propositional/util.cpp
        Propositional/util.h)


add_executable(pr_a Propositional/pr_a.cpp ${UTIL})
add_executable(pr_b Propositional/pr_b.cpp ${UTIL})
add_executable(pr_c Propositional/pr_c.cpp ${UTIL})

add_executable(scheme_definer Propositional/scheme_definer.cpp ${UTIL})