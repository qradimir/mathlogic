//
// Created by radimir on 02.01.16.
//

#ifndef MATHLOGIC_PARSER_H
#define MATHLOGIC_PARSER_H


#include "expression.h"
#include "proof.h"

#include <string>
#include <map>
#include <istream>


class parser {

    std::istream* input;

    size_t lexem;
    std::string last_ref_name;

    bool is_scheme_parsing;

    void nextLex();

    expression parse_implication();
    expression parse_disjunction();
    expression parse_conjunction();
    expression parse_negation();
    expression parse_reference();

    expression parse_variable_ref();
    expression parse_expression_link_ref();
public:

    parser();

    ~parser();

    expression parse(std::string const &str, bool is_scheme_parsing = false);
    expression parse(std::istream& input, bool is_scheme_parsing = false);

    proof parse_proof(std::istream& input);
};


#endif //MATHLOGIC_PARSER_H
