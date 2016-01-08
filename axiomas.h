//
// Created by radimir on 08.01.16.
//

#ifndef MATHLOGIC_AXIOMAS_H
#define MATHLOGIC_AXIOMAS_H

#include "parser.h"
#include <vector>

static std::string axioma_names[]{
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

std::vector<expression const*> get_axiomas() {
    parser axioma_parser;
    std::vector<expression const*> axiomas;
    for (int i = 0; i < 10; ++i) {
        axiomas.push_back(axioma_parser.parse(axioma_names[i], true));
    }
    return axiomas;
};



#endif //MATHLOGIC_AXIOMAS_H
