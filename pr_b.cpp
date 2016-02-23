//
// Created by radimir on 23.02.16.
//

#include <iostream>
#include <fstream>
#include "parser.h"

#define INIT_TIME clock_t _time = clock();
#define LOG_TIME_DELTA(str) std::cerr << (double)(clock() - _time) / CLOCKS_PER_SEC << (str) <<'\n'; _time = clock();

int main ( int argc, char *argv[] ) {
    if (argc < 3) {
        std::cerr << "enter a input & output filename\n";
        return -1;
    }
    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);
    INIT_TIME
    proof pr{input};
    LOG_TIME_DELTA(" - time to parse")
    pr.deduce();
    LOG_TIME_DELTA(" - time to deduce")
    output << pr.annotation(false);

    release_variables();
    release_expression_links();

    return 0;
}