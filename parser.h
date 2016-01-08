//
// Created by radimir on 02.01.16.
//

#ifndef MATHLOGIC_PARSER_H
#define MATHLOGIC_PARSER_H


#include "expression.h"

#include <string>
#include <map>



class parser {


    std::string str;

    size_t index;
    size_t lexem;
    std::string last_ref_name;

    bool is_scheme_parsing;

    void nextLex();

    expression const* parse_implication();
    expression const* parse_disjunction();
    expression const* parse_conjunction();
    expression const* parse_negation();
    expression const* parse_reference();

    expression const* parse_variable_ref();
    expression const* parse_expression_link_ref();
public:

    parser();

    ~parser();

    expression const* parse(std::string const &str, bool is_scheme_parsing = false);
};


#endif //MATHLOGIC_PARSER_H
