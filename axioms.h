//
// Created by radimir on 08.01.16.
//

#ifndef MATHLOGIC_AXIOMAS_H
#define MATHLOGIC_AXIOMAS_H

#include "expression.h"
#include <vector>
#include <string>

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

std::vector<expression> get_axioms();

int is_axiom(expression const& expr);


#endif //MATHLOGIC_AXIOMAS_H
