//
// Created by radimir on 24.02.16.
//

#include <iostream>
#include "expression.h"
#include "parser.h"
#include "util.h"
#include <fstream>

int main (int argc, char *argv[] ) {
    if (argc < 2) {
        std::cerr << "Enter a expression to proof\n";;
    }
    if (argc > 2 && argv[2] == "-d") {
        set_debug(&std::cerr);
    }
    expression e = get_expression_parser().parse(argv[1]);
    std::vector<variable *> vars = e.get_variables();
    bit_tuple values{vars.size()};
    bool all = true;
    values.do_while(
            [&all, &vars, &e](bool const*v, size_t c) -> bool {
                for (size_t k = 0; k < c; ++k) {
                    vars[k]->value = v[k];
                }
                return all &= e.get()();
            }
    );
    if (!all) {
        std::cout << "Высказывание ложно при " << vars[0]->to_string();
        for (size_t i = 1; i < vars.size(); ++i) {
            std::cout << ", " << vars[i]->to_string();
        }
        std::cout << '\n';
    } else {
        INIT_TIME
        proof pr{e};
        LOG_TIME_DELTA(" - building proof")
        pr.simplify();
        LOG_TIME_DELTA(" - simplifying proof")
        std::cout << pr;
    }
    release();
    return 0;
}