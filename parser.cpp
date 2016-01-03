//
// Created by radimir on 02.01.16.
//

#include <assert.h>
#include "parser.h"

#define LEXEM_END 0
#define LEXEM_OBR 1
#define LEXEM_CBR 2
#define LEXEM_VAR_NAME 3
#define LEXEM_IMPLICATION 4
#define LEXEM_CONJUNCTION 5
#define LEXEM_DISJUNCTION 6
#define LEXEM_NEGATION 7


void parser::nextLex() {
    switch(str[index]) {
        case '(' :
            lexem = LEXEM_OBR;
            break;
        case ')' :
            lexem = LEXEM_CBR;
            break;
        case '-' :
            lexem = LEXEM_IMPLICATION;
            index++;
            break;
        case '&' :
            lexem = LEXEM_CONJUNCTION;
            break;
        case '|' :
            lexem = LEXEM_DISJUNCTION;
            break;
        case '!' :
            lexem = LEXEM_NEGATION;
            break;
        case '\0' :
            lexem = LEXEM_END;
            break;
        default:
            assert((str[index] >= 'A' && str[index] <= 'Z'));

            lexem = LEXEM_VAR_NAME;
            size_t t = index;
            index++;
            while(str[index] >= 'A' && str[index] <= 'Z' || str[index] >= '0' && str[index] <= '9') {
                index++;
            }
            last_var_name = str.substr(t, index - t);
            return;
    }
    index++;
}

expression parser::parse_implication() {
    expression impl = parse_disjunction();
    if (lexem == LEXEM_IMPLICATION) {
        nextLex();
        return make_implication(impl, parse_implication());
    } else {
        return impl;
    }
}

expression parser::parse_disjunction() {
    expression disj = parse_conjunction();
    while (lexem == LEXEM_DISJUNCTION) {
        nextLex();
        disj = make_disjunction(disj, parse_conjunction());
    }
    return disj;
}

expression parser::parse_conjunction() {
    expression conj = parse_negation();
    while (lexem == LEXEM_CONJUNCTION) {
        nextLex();
        conj = make_conjunction(conj, parse_negation());
    }
    return conj;
}

expression parser::parse_negation() {
    if (lexem == LEXEM_NEGATION) {
        nextLex();
        return make_negation(parse_negation());
    }
    if (lexem == LEXEM_OBR) {
        nextLex();
        expression ret = parse_implication();
        nextLex();
        return ret;
    }
    return parse_variable_ref();
}

expression parser::parse_variable_ref() {
    variable* var;
    auto ptr = vars.find(last_var_name);
    if (ptr == vars.end()) {
        var =  new variable(last_var_name);
        vars.insert(std::make_pair(last_var_name, var));
    } else {
        var = ptr->second;
    }
    return make_variable_ref(*var);
}

parser::parser() {
}

parser::~parser() {
    for(auto it = vars.begin(); it != vars.end(); it++) {
        delete(it->second);
    }
}

expression parser::parse(std::string const& str) {
    this->str = str;
    index = 0;
    last_var_name = "";
    nextLex();
    return parse_implication();
}

variable *parser::get_variable(std::string const& str) {
    auto ptr = vars.find(str);
    if (ptr == vars.end())
    {
        return nullptr;
    }
    return ptr->second;
}