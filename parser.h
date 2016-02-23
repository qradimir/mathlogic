//
// Created by radimir on 02.01.16.
//

#ifndef MATHLOGIC_PARSER_H
#define MATHLOGIC_PARSER_H


#include "expression.h"
#include "proof.h"
#include "expression_scheme.h"

#include <string>
#include <map>
#include <istream>
#include <sstream>

template <typename T>
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

    void nextLex(){
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

    T parse_implication() {
        T impl = parse_disjunction();
        if (lexem == LEXEM_IMPLICATION) {
            nextLex();
            return build_implication(impl, parse_implication());
        } else {
            return impl;
        }
    }

    T parse_disjunction() {
        T disj = parse_conjunction();
        while (lexem == LEXEM_DISJUNCTION) {
            nextLex();
            disj = build_disjunction(disj, parse_conjunction());
        }
        return disj;
    }

    T parse_conjunction() {
        T conj = parse_negation();
        while (lexem == LEXEM_CONJUNCTION) {
            nextLex();
            conj = build_conjunction(conj, parse_negation());
        }
        return conj;
    }

    T parse_negation() {
        if (lexem == LEXEM_NEGATION) {
            nextLex();
            return build_negation(parse_negation());
        }
        if (lexem == LEXEM_OBR) {
            nextLex();
            T ret = parse_implication();
            nextLex();
            return ret;
        }
        T t = parse_reference(last_ref_name);
        nextLex();
        return t;
    }

    typedef std::function<T(std::string const&)> ref_parser;
    typedef std::function<T(T const&)> op1_builder;
    typedef std::function<T(T const&, T const&)> op2_builder;

    ref_parser parse_reference;

    op2_builder build_implication;
    op2_builder build_disjunction;
    op2_builder build_conjunction;

    op1_builder build_negation;

public:

    parser(ref_parser ref_parser, op2_builder impl, op2_builder disj, op2_builder conj, op1_builder neg)
        : parse_reference(ref_parser),
          build_implication(impl),
          build_disjunction(disj),
          build_conjunction(conj),
          build_negation(neg) {}

    virtual ~parser() {};

    T parse(std::string const &str) {
        std::istringstream input{str};
        return parse(input);
    }

    T parse(std::istream& input) {
        this->input = &input;
        last_ref_name = "";
        nextLex();
        return parse_implication();
    }
};

parser<expression> get_expression_parser();
parser<expression_scheme> get_scheme_parser();

#endif //MATHLOGIC_PARSER_H
