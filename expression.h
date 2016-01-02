//
// Created by radimir on 02.01.16.
//

#ifndef MATHLOGIC_EXPRESSION_H
#define MATHLOGIC_EXPRESSION_H

#include <functional>

class connective;

class expression {

    connective const* conn;

    size_t sub_count;
    expression* sub;

    void copy(connective const* conn, size_t sub_count, expression const* sub) {
        this->conn = conn;
        this->sub_count = sub_count;
        this->sub = new expression[sub_count];
        for (int i = 0; i < sub_count; ++i) {
            this->sub[i] = sub[i];
        }
    }

public:

    expression();

    expression(connective const* conn, size_t  sub_count, expression const* sub);

    expression(expression const& other);

    bool operator()() const;


    inline size_t get_sub_count() const {
        return sub_count;
    }

    inline expression const& get_sub(size_t i) const {
        if (i >= sub_count)
            throw -1;

        return sub[i];
    }

    void operator=(expression const& other);


    std::string to_string();
    std::string to_bounded_string(size_t priority);

    friend std::ostream & operator<<(std::ostream & out, expression a) {
        return out << a.to_string();
    }
};

class connective {
public:
    size_t priority;
    std::function<bool(bool*)> evaluator;
    std::function<std::string(expression*)> str_getter;

    connective(std::function<bool(bool*)> evaluator, std::function<std::string(expression*)> str_getter, size_t priority);

};

class variable {


public:
    bool value;
    std::string name;
    connective conn;


    variable(std::string const& name);

    variable(std::string const& name, bool value);

};



expression make_disjunction(expression const& left, expression const& right);
expression make_conjunction(expression const& left, expression const& right);
expression make_implication(expression const& left, expression const& right);

expression make_negation(expression const& expr);

expression make_variable_ref(variable const& var);

#endif //MATHLOGIC_EXPRESSION_H
