//
// Created by radimir on 02.01.16.
//

#include <sstream>
#include <iostream>
#include "parser.h"
#include "expression.h"

void parser::nextLex() {
    char c;
    if (!input->get(c)) {
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
            if ((c < 'A' || c > 'Z')) {
                throw "unexpected symbols while parsing\n";
            }

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
            return S_IMPL(impl, parse_implication());
        } else {
            return impl;
        }
    }

expression parser::parse_disjunction() {
    expression disj = parse_conjunction();
    while (lexem == LEXEM_DISJUNCTION) {
        nextLex();
        disj = S_DISJ(disj, parse_conjunction());
    }
    return disj;
}

expression parser::parse_conjunction() {
    expression conj = parse_negation();
    while (lexem == LEXEM_CONJUNCTION) {
        nextLex();
        conj = S_CONJ(conj, parse_negation());
    }
    return conj;
}

expression parser::parse_negation() {
    if (lexem == LEXEM_NEGATION) {
        nextLex();
        return S_NEG(parse_negation());
    }
    if (lexem == LEXEM_OBR) {
        nextLex();
        expression ret = parse_implication();
        nextLex();
        return ret;
    }
    expression t = make_reference(last_ref_name);
    nextLex();
    return std::move(t);
}

expression parser::parse(std::string const &str) {
    std::istringstream input{str};
    return parse(input);
}

expression parser::parse(std::istream& input) {
    this->input = &input;
    last_ref_name = "";
    nextLex();
    return parse_implication();
}