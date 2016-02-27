//
// Created by radimir on 25.02.16.
//

#include <iostream>
#include <fstream>
#include <string.h>
#include "parser.h"

/*
 *  parser to make defines like in axioms.cpp
 */
parser<std::string> p{
        [](std::string const &s) {
            return "S_REF(" + s + ")";
        },
        [](std::string const& left, std::string const& right) {
            return "S_IMPL(" + left + "," + right + ")";
        },
        [](std::string const& left, std::string const& right) {
            return "S_DISJ(" + left + "," + right + ")";
        },
        [](std::string const& left, std::string const& right) {
            return "S_CONJ(" + left + "," + right + ")";
        },
        [](std::string const& t) {
            return "S_NEG(" + t + ")";
        }
};
//TODO Fix bugs
int main(int argc, char *argv[]) {
    assert(argc > 1);
    if (strcmp(argv[1], "-f") == 0) {
        std::ifstream in{argv[2]};
        while (!in.eof()) {
            std::string res = p.parse(in);
            std::cout << res << '\n';
        }
        return 0;
    }
    if (strcmp(argv[1], "-h") == 0) {
        std::ifstream in{argv[2]};
        while (!in.eof()) {
            std::string str;
            std::getline(in, str);
            std::string res = p.parse(str);
            std::cout << "//" << str << "\n\t" << res << ",\n";
        }
        return 0;
    }
    for (int i = 1; i < argc; ++i) {
        std::cout << p.parse(argv[i]) << '\n';
    }
}