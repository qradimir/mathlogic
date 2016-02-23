//
// Created by radimir on 09.01.16.
//

#include <iostream>
#include "axioms.h"

std::vector<expression_scheme> axioms{};
std::vector<expression_scheme> self_impl_proof{};

bool initialized= false;

void init_axioms() {
    if (initialized)
        return;

    for (int i = 0; i < 10; ++i) {
        axioms.push_back(axiom_parser.parse(axiom_names[i]));
    }

    for (int i = 0; i < 5; ++i) {
        self_impl_proof.push_back(axiom_parser.parse(a_impl_a_proof_s[i]));
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

std::vector<expression> get_a_impl_a_proof(expression const &sub) {
    if (!initialized) {
        init_axioms();
    }
    std::vector<expression> ret{5};
    std::map<std::string, expression> m{};
    m["A"] = sub;
    for (int i = 0; i < 5; ++i) {
        ret.push_back(self_impl_proof[i].get_expression(m));
    }
    return std::vector<expression>();
}