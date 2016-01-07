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

expression const* parser::parse_implication() {
    expression const* impl = parse_disjunction();
    if (lexem == LEXEM_IMPLICATION) {
        nextLex();
        return new operation(&implication, expression_storage(impl, parse_disjunction()));
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
    return parse_variable_ref();
}

expression const* parser::parse_variable_ref() {
    variable* var;
    auto ptr = vars.find(last_var_name);
    if (ptr == vars.end()) {
        var =  new variable(last_var_name);
        vars.insert(std::make_pair(last_var_name, var));
    } else {
        var = ptr->second;
    }
    return new variable_ref(var);
}

parser::parser() {
}

parser::~parser() {
    for(auto it = vars.begin(); it != vars.end(); it++) {
        delete(it->second);
    }
}

expression const* parser::parse(std::string const& str) {
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