//
// Created by radimir on 08.01.16.
//

#include <iostream>
#include <fstream>
#include "parser.h"
#include "axioms.h"

int main ( int argc, char *argv[] ) {
    if (argc < 2) {
        std::cerr << "enter a filename that contains proof\n";
        return -1;
    }
    std::ifstream input(argv[1]);
    parser p;
    std::cout << p.parse(input)->to_string();
    return 0;
}