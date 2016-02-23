//
// Created by radimir on 09.01.16.
//

#include <iostream>
#include "axioms.h"
#include "parser.h"

std::vector<expression_scheme> axioms{};
bool initialized= false;

void init_axioms() {
    if (initialized)
        return;

    parser<expression_scheme> axiom_parser = get_scheme_parser();
    for (int i = 0; i < 10; ++i) {
        axioms.push_back(axiom_parser.parse(axiom_names[i]));
    }

    initialized = true;
}

std::vector<expression_scheme> get_axioms() {
    if (!initialized) {
        init_axioms();
    }
    return axioms;
}

int is_axiom(expression const& expr) {
    if (!initialized) {
        init_axioms();
    }
    for (int i = 0; i < axioms.size(); ++i) {
        if (axioms[i].match(expr))
            return i;
    }
    return -1;
}
