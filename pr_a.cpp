//
// Created by radimir on 08.01.16.
//

#include <iostream>
#include <fstream>
#include "parser.h"

#define INIT_TIME clock_t _time = clock();
#define LOG_TIME_DELTA(str) std::cout << (double)(clock() - _time) / CLOCKS_PER_SEC << (str) <<'\n'; _time = clock();

int main ( int argc, char *argv[] ) {
    if (argc < 3) {
        std::cerr << "enter a input & output filename\n";
        return -1;
    }
    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);
    INIT_TIME
    parser p;
    proof _proof = p.parse_proof(input);
    LOG_TIME_DELTA(" - time to parse")
    _proof.annotate();
    LOG_TIME_DELTA(" - time to annotate")
    output << _proof.annotation();
    release();
    return 0;
}