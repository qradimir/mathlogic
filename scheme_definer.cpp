//
// Created by radimir on 25.02.16.
//

#include <iostream>
#include <fstream>
#include <string.h>
#include "parser.h"
#include "expression.h"

/*
 *  build defines like in axioms.cpp
 */
std::string build(expression const& e);

std::string build(std::shared_ptr<operation> op) {
    static std::function<std::string(expression const *)> builder[]{
            [](expression const *storage) {
                return "S_IMPL(" + build(storage[0]) + "," + build(storage[1]) + ")";
            },
            [](expression const *storage) {
                return "S_DISJ(" + build(storage[0]) + "," + build(storage[1]) + ")";
            },
            [](expression const *storage) {
                return "S_CONJ(" + build(storage[0]) + "," + build(storage[1]) + ")";
            },
            [](expression const *storage) {
                return "S_NEG(" + build(storage[0]) + ")";
            }
    };
    return builder[op->connective_type](op->storage);
}

std::string build(std::shared_ptr<reference> ref) {
    return "S_REF(" + ref->name + ")";
}

std::string build(expression const &e) {
    switch (e->type()) {
        case TYPE_OP :
            return build(get_op(e));
        case TYPE_REF :
            return build(get_ref(e));
        default:
            return "";
    }
}

parser p{};

//TODO Fix bugs
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "usage: -f <input_file> | -h <input_file> | [expresison ...]\n";
        return 0;
    }
    if (strcmp(argv[1], "-f") == 0) {
        std::ifstream in{argv[2]};
        while (!in.eof()) {
            std::string res = build(p.parse(in));
            std::cout << res << '\n';
        }
        return 0;
    }
    if (strcmp(argv[1], "-h") == 0) {
        std::ifstream in{argv[2]};
        while (!in.eof()) {
            std::string str;
            std::getline(in, str);
            std::string res = build(p.parse(str));
            std::cout << "//" << str << "\n\t" << res << ",\n";
        }
        return 0;
    }
    for (int i = 1; i < argc; ++i) {
        std::cout << build(p.parse(argv[i])) << '\n';
    }
}