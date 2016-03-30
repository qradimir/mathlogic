//
// Created by radimir on 23.02.16.
//

#include <iostream>
#include <fstream>
#include "parser.h"
#include "proof.h"
#include "util.h"

int main ( int argc, char *argv[] ) {
    if (argc < 2) {
        std::cerr << "enter a input filename\n";
        return -1;
    }
    std::ifstream input(argv[1]);
    set_debug(argc > 2 && strcmp(argv[2], "-d"));
    INIT_TIME
    proof pr{input};
    LOG_TIME_DELTA(" - time to parse")
    pr.deduce();
    LOG_TIME_DELTA(" - time to deduce")
    std::cout << pr;

    return 0;
}