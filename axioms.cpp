//
// Created by radimir on 09.01.16.
//

#include <iostream>
#include "axioms.h"

int is_axiom(expression const& expr) {
    for (int i = 0; i < 10; ++i) {
        if (AXIOM[i].match(expr))
            return i;
    }
    return -1;
}

std::vector<expression> get_a_impl_a_proof(expression const &sub) {
    std::vector<expression> ret{};
    std::map<std::string, expression> m{};
    m["A"] = sub;
    for (int i = 0; i < 5; ++i) {
        ret.push_back(AA_IMPL[i].get_expression(m));
    }
    return ret;
}