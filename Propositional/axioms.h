//
// Created by radimir on 08.01.16.
//

#ifndef MATHLOGIC_AXIOMAS_H
#define MATHLOGIC_AXIOMAS_H

#include "expression.h"
#include <vector>

std::vector<expression> const &get_not_vsproof(bool arg0);
std::vector<expression> const &get_impl_vsproof(bool arg0, bool arg1);
std::vector<expression> const &get_conj_vsproof(bool arg0, bool arg1);
std::vector<expression> const &get_disj_vsproof(bool arg0, bool arg1);

std::vector<expression> const &aa_deduction();
std::vector<expression> const &ax_deduction();
std::vector<expression> const &mp_deduction();

std::vector<expression> const &suppose_reduce_proof();

std::vector<expression> get_sub_list(std::vector<expression> const &schemes,
                                     expression_link::holder const &m);
int is_axiom(expression const& expr);


#endif //MATHLOGIC_AXIOMAS_H
