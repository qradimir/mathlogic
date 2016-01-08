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

expression const* parser::parse_implication() {
    expression const* impl = parse_disjunction();
    if (lexem == LEXEM_IMPLICATION) {
        nextLex();
        return new operation(&implication, expression_storage(impl, parse_implication()));
    } else {
        return impl;
    }
}

expression const* parser::parse_disjunction() {
    expression const* disj = parse_conjunction();
    while (lexem == LEXEM_DISJUNCTION) {
        nextLex();
        disj = new operation(&disjunction, expression_storage(disj, parse_conjunction()));
    }
    return disj;
}

expression const* parser::parse_conjunction() {
    expression const* conj = parse_negation();
    while (lexem == LEXEM_CONJUNCTION) {
        nextLex();
        conj = new operation(&conjunction, expression_storage(conj, parse_negation()));
    }
    return conj;
}

expression const* parser::parse_negation() {
    if (lexem == LEXEM_NEGATION) {
        nextLex();
        return new operation(&negation, expression_storage(parse_negation()));
    }
    if (lexem == LEXEM_OBR) {
        nextLex();
        expression const* ret = parse_implication();
        nextLex();
        return ret;
    }
    return parse_reference();
}


inline expression const *parser::parse_reference() {
    return (is_scheme_parsing ? parse_expression_link_ref() : parse_variable_ref());
}

expression const* parser::parse_variable_ref() {
    auto ptr = find_variable(last_ref_name);
    if (ptr == nullptr) {
        ptr = new variable(last_ref_name);
    }
    nextLex();
    return new variable_ref(ptr);
}


expression const *parser::parse_expression_link_ref() {
    auto ptr = find_expression_link(last_ref_name);
    if (ptr == nullptr) {
        ptr =  new expression_link(last_ref_name);
    }
    nextLex();
    return new expression_link_ref(ptr);
}

parser::parser() {
}

parser::~parser() {
}

expression const* parser::parse(std::string const &str, bool is_scheme_parcing) {
    std::istringstream* input = new std::istringstream(str);
    auto ptr = parse(*input, is_scheme_parcing);
    delete(input);
    return ptr;
}

expression const* parser::parse(std::istream& input, bool is_scheme_parsing) {
    this->input = &input;
    last_ref_name = "";
    this->is_scheme_parsing = is_scheme_parsing;
    nextLex();
    auto ptr = parse_implication();
    add_to_release(ptr);
    return ptr;
}
