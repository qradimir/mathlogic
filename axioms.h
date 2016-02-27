//
// Created by radimir on 08.01.16.
//

#ifndef MATHLOGIC_AXIOMAS_H
#define MATHLOGIC_AXIOMAS_H

#include "expression.h"
#include "expression_scheme.h"
#include <vector>

std::vector<expression_scheme> const &get_not_vsproof(bool arg0);
std::vector<expression_scheme> const &get_impl_vsproof(bool arg0, bool arg1);
std::vector<expression_scheme> const &get_conj_vsproof(bool arg0, bool arg1);
std::vector<expression_scheme> const &get_disj_vsproof(bool arg0, bool arg1);

std::vector<expression_scheme> const &aa_deduction();
std::vector<expression_scheme> const &ax_deduction();
std::vector<expression_scheme> const &mp_deduction();

std::vector<expression_scheme> const &suppose_reduce_proof();

std::vector<expression> get_sub_list(std::vector<expression_scheme> schemes, std::map<std::string, expression> const &m);
int is_axiom(expression const& expr);


#endif //MATHLOGIC_AXIOMAS_H
