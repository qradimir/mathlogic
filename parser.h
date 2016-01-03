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

    expression parse_implication();
    expression parse_disjunction();
    expression parse_conjunction();
    expression parse_negation();
    expression parse_variable_ref();

public:

    parser();

    ~parser();

    variable * get_variable(std::string const& str);

    expression parse(std::string const& str);
};


#endif //MATHLOGIC_PARSER_H
