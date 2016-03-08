//
// Created by radimir on 24.02.16.
//

#include <iostream>
#include "expression.h"
#include "parser.h"
#include "proof.h"
#include <fstream>


std::string info(variable::holder::iterator var) {
    return var->first + "=" +  (var->second ? "И" : "Л");
}

int main (int argc, char *argv[] ) {
    if (argc < 2) {
        std::cerr << "Enter a expression to proof\n";;
        return 1;
    }
    set_debug(argc > 2 && strcmp(argv[2], "-d"));
    DEBUG("debug enabled");
    expression e;
    try {
        e = parser{}.parse(argv[1]);
    } catch (std::string &e) {
        std::cerr << "erorr :" << e << '\n';
        return 1;
    }
    variable::holder vars = e.values<bool>();
    bit_tuple values{vars.size()};
    bool all = true;
    values.do_while(
            [&all, &vars, &e](bool const*v, size_t c) -> bool {
                int k = 0;
                for (variable::holder::iterator it = vars.begin(); it != vars.end(); ++it) {
                    it->second = v[k++];
                }
                return all &= e(vars);
            }
    );
    if (!all) {
        std::cout << "Высказывание ложно при " << info(vars.begin());
        for (auto it = ++vars.begin(); it != vars.end(); ++it) {
            std::cout << ", " << info(it);
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
    return 0;
}