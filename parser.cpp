//
// Created by radimir on 02.01.16.
//

#include <sstream>
#include <iostream>
#include "parser.h"

parser<expression> get_expression_parser() {
    return parser<expression>(
            [](std::string s) {
                auto ptr = find_variable(s);
                if (ptr == nullptr) {
                    ptr = new variable(s);
                }
                return make_variable_ref(ptr);
            },
            [](expression const& left, expression const& right) {
                return make_operation(get_implication(), left, right);
            },
            [](expression const& left, expression const& right) {
                return make_operation(get_disjunction(), left, right);
            },
            [](expression const& left, expression const& right) {
                return make_operation(get_conjunction(), left, right);
            },
            [](expression const& t) {
                return make_operation(get_negation(), t);
            }
    );
}

parser<expression_scheme> get_scheme_parser() {
    return parser<expression_scheme>(
            [](std::string s) {
                auto ptr = find_expression_link(s);
                if (ptr == nullptr) {
                    ptr = new expression_link(s);
                }
                return make_expression_link_ref(ptr);
            },
            [](expression_scheme const& left, expression_scheme const& right) {
                return make_operation_scheme(get_implication(), left, right);
            },
            [](expression_scheme const& left, expression_scheme const& right) {
                return make_operation_scheme(get_disjunction(), left, right);
            },
            [](expression_scheme const& left, expression_scheme const& right) {
                return make_operation_scheme(get_conjunction(), left, right);
            },
            [](expression_scheme const& t) {
                return make_operation_scheme(get_negation(), t);
            }
    );
}
