//
// Created by radimir on 02.01.16.
//

#ifndef MATHLOGIC_PARSER_H
#define MATHLOGIC_PARSER_H


#include "expression.h"

#include <string>
#include <map>



class parser {

    std::map<std::string, variable*> vars;

    std::string str;

    size_t index;
    size_t lexem;
    std::string last_var_name;

    void nextLex();

    expression const* parse_implication();
    expression const* parse_disjunction();
    expression const* parse_conjunction();
    expression const* parse_negation();
    expression const* parse_variable_ref();

public:

    parser();

    ~parser();

    variable * get_variable(std::string const& str);

    expression const* parse(std::string const& str);
};


#endif //MATHLOGIC_PARSER_H
