//
// Created by radimir on 08.01.16.
//

#include <iostream>
#include <fstream>
#include "parser.h"

int main ( int argc, char *argv[] ) {
    if (argc < 2) {
        std::cerr << "enter a filename that contains proof\n";
        return -1;
    }
    std::ifstream input(argv[1]);
    parser p;
    proof _proof = p.parse_proof(input);
    _proof.annotate();
    std::cout << _proof.annotation();
    release();
    return 0;
}