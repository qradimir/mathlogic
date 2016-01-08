//
// Created by radimir on 02.01.16.
//

#include <assert.h>
#include <sstream>
#include "parser.h"

#define LEXEM_END 0
#define LEXEM_NEW_LINE 1
#define LEXEM_COMMA 2
#define LEXEM_SEPARATOR 3
#define LEXEM_OBR 4
#define LEXEM_CBR 5
#define LEXEM_VAR_NAME 6
#define LEXEM_IMPLICATION 10
#define LEXEM_CONJUNCTION 11
#define LEXEM_DISJUNCTION 12
#define LEXEM_NEGATION 13


void parser::nextLex() {
    char c;
    while (input->get(c) && c == ' ') {
    }
    if (!input) {
        lexem = LEXEM_END;
        return;
    }

    switch(c) {
        case '(' :
            lexem = LEXEM_OBR;
            break;
        case ')' :
            lexem = LEXEM_CBR;
            break;
        case '-' :
            lexem = LEXEM_IMPLICATION;
            input->get();
            break;
        case '&' :
            lexem = LEXEM_CONJUNCTION;
            break;
        case '|' :
            if (input->peek() == '-') {
                lexem = LEXEM_SEPARATOR;
                input->get();
                break;
            }
            lexem = LEXEM_DISJUNCTION;
            break;
        case '!' :
            lexem = LEXEM_NEGATION;
            break;
        case ',' :
            lexem = LEXEM_COMMA;
            break;
        case '\n' :
            lexem = LEXEM_NEW_LINE;
            break;
        case '\0' :
            lexem = LEXEM_END;
            break;
        default:
            assert((c >= 'A' && c <= 'Z'));

            lexem = LEXEM_VAR_NAME;
            last_ref_name = c;
            char symb;
            while(input->get(symb) && symb >= 'A' && symb <= 'Z' || symb >= '0' && symb <= '9') {
                last_ref_name += symb;
            }
            if (input) {
                input->putback(symb);
            }
    }
}

expression parser::parse_implication() {
    expression impl = parse_disjunction();
    if (lexem == LEXEM_IMPLICATION) {
        nextLex();
        return make_operation(&implication, impl, parse_implication());
    } else {
        return impl;
    }
}

expression parser::parse_disjunction() {
    expression disj = parse_conjunction();
    while (lexem == LEXEM_DISJUNCTION) {
        nextLex();
        disj = make_operation(&disjunction, disj, parse_conjunction());
    }
    return disj;
}

expression parser::parse_conjunction() {
    expression conj = parse_negation();
    while (lexem == LEXEM_CONJUNCTION) {
        nextLex();
        conj = make_operation(&conjunction, conj, parse_negation());
    }
    return conj;
}

expression parser::parse_negation() {
    if (lexem == LEXEM_NEGATION) {
        nextLex();
        return make_operation(&negation, parse_negation());
    }
    if (lexem == LEXEM_OBR) {
        nextLex();
        expression ret = parse_implication();
        nextLex();
        return ret;
    }
    return parse_reference();
}


inline expression parser::parse_reference() {
    return (is_scheme_parsing ? parse_expression_link_ref() : parse_variable_ref());
}

expression parser::parse_variable_ref() {
    auto ptr = find_variable(last_ref_name);
    if (ptr == nullptr) {
        ptr = new variable(last_ref_name);
    }
    nextLex();
    return make_variable_ref(ptr);
}


expression parser::parse_expression_link_ref() {
    auto ptr = find_expression_link(last_ref_name);
    if (ptr == nullptr) {
        ptr =  new expression_link(last_ref_name);
    }
    nextLex();
    return make_expression_link_ref(ptr);
}

parser::parser() {
}

parser::~parser() {
}

expression parser::parse(std::string const &str, bool is_scheme_parcing) {
    std::istringstream* input = new std::istringstream(str);
    expression _ = parse(*input, is_scheme_parcing);
    delete(input);
    return _;
}

expression parser::parse(std::istream& input, bool is_scheme_parsing) {
    this->input = &input;
    last_ref_name = "";
    this->is_scheme_parsing = is_scheme_parsing;
    nextLex();
    return parse_implication();
}
