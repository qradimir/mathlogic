//
// Created by radimir on 02.01.16.
//

#ifndef MATHLOGIC_PARSER_H
#define MATHLOGIC_PARSER_H


#include <string>
#include <map>
#include <istream>
#include <sstream>

class proof;
class expression;

class parser {
    friend class proof;
protected:

    static const size_t LEXEM_END  = 0;
    static const size_t LEXEM_NEW_LINE  = 1;
    static const size_t LEXEM_COMMA = 2;
    static const size_t LEXEM_SEPARATOR = 3;
    static const size_t LEXEM_OBR = 4;
    static const size_t LEXEM_CBR = 5;
    static const size_t LEXEM_VAR_NAME = 6;
    static const size_t LEXEM_IMPLICATION = 10;
    static const size_t LEXEM_CONJUNCTION = 11;
    static const size_t LEXEM_DISJUNCTION = 12;
    static const size_t LEXEM_NEGATION = 13;

    std::istream* input;

    size_t lexem;
    std::string last_ref_name;

    void nextLex();

    expression parse_implication();
    expression parse_disjunction();
    expression parse_conjunction();
    expression parse_negation();

public:

    expression parse(std::string const &str);
    expression parse(std::istream& input);
};

#endif //MATHLOGIC_PARSER_H
