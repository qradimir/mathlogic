//
// Created by radimir on 25.02.16.
//

#include <iostream>
#include <fstream>
#include <string.h>
#include "parser.h"

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

int main(int argc, char *argv[]) {
    assert(argc > 1);
    if (strcmp(argv[1],"-f") == 0) {
        std::ifstream in{argv[2]};
        while (true) {
            std::string res = p.parse(in);
            if (in.eof()) {
                return 0;
            }
            std::cout << res << '\n';
        }
    }
    for (int i = 1; i < argc; ++i) {
        std::cout << p.parse(argv[i]) << '\n';
    }
}