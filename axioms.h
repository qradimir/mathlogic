//
// Created by radimir on 08.01.16.
//

#ifndef MATHLOGIC_AXIOMAS_H
#define MATHLOGIC_AXIOMAS_H

#include "expression.h"
#include "expression_scheme.h"
#include "parser.h"
#include <vector>
#include <string>

static parser<expression_scheme> axiom_parser = get_scheme_parser();
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
static std::string a_impl_a_proof_s[]{
        "A->A->A",
        "(A->A->A)->(A->(A->A)->A)->(A->A)",
        "(A->(A->A)->A)->(A->A)",
        "A->(A->A)->A",
        "A->A"
};
static expression_scheme mp_scheme_0 = axiom_parser.parse("Y->X");
static expression_scheme mp_scheme_1 = axiom_parser.parse("(X->Y->Z)->(X->Z)");
static expression_scheme mp_mp_scheme_1 = axiom_parser.parse("X->Z");


std::vector<expression_scheme> get_axioms();
std::vector<expression> get_a_impl_a_proof(expression const& sub);

int is_axiom(expression const& expr);


#endif //MATHLOGIC_AXIOMAS_H
