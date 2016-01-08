//
// Created by radimir on 08.01.16.
//

#ifndef MATHLOGIC_AXIOMAS_H
#define MATHLOGIC_AXIOMAS_H

#include "parser.h"
#include <vector>

static std::string axiom_names[]{
    "X->Y->X",
    "(X->Y)->(X->Y->Z)->(X->Z)",
    "X->Y->X&Y",
    "X&Y->X",
    "X&Y->Y",
    "X->X|Y",
    "Y->X|Y",
    "(X->Z)->(Y->Z)->(X|Y->Z)",
    "(X->Y)->(X->!Y)->!X",
    "!!X->X"
};

std::vector<expression> get_axioms() {
    parser axiom_parser;
    std::vector<expression> axioms;
    for (int i = 0; i < 10; ++i) {
        axioms.push_back(axiom_parser.parse(axiom_names[i], true));
    }
    return axioms;
};



#endif //MATHLOGIC_AXIOMAS_H
