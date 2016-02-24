//
// Created by radimir on 02.01.16.
//

#include <sstream>
#include <iostream>
#include "parser.h"

parser<expression> get_expression_parser() {
    return parser<expression>(
            [](std::string s) {
                return make_variable_ref(s);
            },
            [](expression const& left, expression const& right) {
                return E_IMPL(left, right);
            },
            [](expression const& left, expression const& right) {
                return E_DISJ(left, right);
            },
            [](expression const& left, expression const& right) {
                return E_CONJ(left, right);
            },
            [](expression const& t) {
                return E_NEG(t);
            }
    );
}

parser<expression_scheme> get_scheme_parser() {
    return parser<expression_scheme>(
            [](std::string s) {
                return make_expression_link_ref(s);
            },
            [](expression_scheme const& left, expression_scheme const& right) {
                return S_IMPL(left, right);
            },
            [](expression_scheme const& left, expression_scheme const& right) {
                return S_DISJ(left, right);
            },
            [](expression_scheme const& left, expression_scheme const& right) {
                return S_CONJ(left, right);
            },
            [](expression_scheme const& t) {
                return S_NEG(t);
            }
    );
}
