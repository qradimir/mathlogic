//
// Created by radimir on 08.01.16.
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
    pr.annotate();
    LOG_TIME_DELTA(" - time to annotate")
    std::cout << pr.annotation();

    return 0;
}