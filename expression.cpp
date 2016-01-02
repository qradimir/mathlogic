//
// Created by radimir on 02.01.16.
//

#include "expression.h"


expression::expression() {
    sub = new expression[0];
}

expression::expression(const expression &other)
{
    copy(other.conn, other.sub_count, other.sub);
}

expression::expression(connective const* conn, size_t sub_count, expression const* sub)
{
    copy(conn, sub_count, sub);
}

connective negation(
        [] (bool* args) -> bool { return !args[0]; },
        [] (expression* exprs) -> std::string { return  "!" +  exprs[0].to_bounded_string(1); },
        1
);

connective conjunction(
        [] (bool* args) -> bool { return args[0] & args[1]; },
        [] (expression* exprs) -> std::string { return exprs[0].to_bounded_string(2) + "&" + exprs[1].to_bounded_string(2); },
        2
);

connective disjunction(
        [] (bool* args) -> bool { return args[0] | args[1]; },
        [] (expression* exprs) -> std::string { return exprs[0].to_bounded_string(3) + "|" + exprs[1].to_bounded_string(3); },
        3
);

connective implication(
        [] (bool* args) -> bool { return !args[0] | args[1]; },
        [] (expression* exprs) -> std::string { return exprs[0].to_bounded_string(3) + "->" + exprs[1].to_bounded_string(4); },
        4
);

expression make_conjunction(expression const& left, expression const& right) {
    expression sub[2];
    sub[0] = left;
    sub[1] = right;
    return expression(&conjunction, 2, sub);
}

expression make_disjunction(expression const& left, expression const& right) {
    expression sub[2];
    sub[0] = left;
    sub[1] = right;
    return expression(&disjunction, 2, sub);
}

expression make_implication(expression const &left, expression const &right) {
    expression sub[2];
    sub[0] = left;
    sub[1] = right;
    return expression(&implication, 2, sub);
}

expression make_negation(expression const& expr) {
    return expression(&negation, 1, &expr);
}


void expression::operator=(expression const &other) {
    delete[](sub);
    sub = new expression[other.sub_count];
    copy(other.conn, other.sub_count, other.sub);
}


bool expression::operator()() const {
    bool values[sub_count];
    for (int i = 0; i < sub_count; ++i) {
        values[i] = sub[i]();
    }
    return conn->evaluator(values);
}

expression make_variable_ref(variable const& var) {
    return expression(&var.conn, 0, nullptr);
}

variable::variable(std::string const& name) :
        variable(name, false)
{
}

variable::variable(std::string const& name, bool value) :
        name(name),
        value(value),
        conn(
            [this] (bool*) -> bool { return this->value; },
            [this] (expression*) -> std::string { return this->name; },
            0
        )
{
}

connective::connective(std::function<bool(bool*)> evaluator, std::function<std::string(expression*)> str_getter, size_t priority)
        : evaluator(evaluator), str_getter(str_getter), priority(priority) {
}

std::string expression::to_string() {
    return conn->str_getter(sub);
}

std::string expression::to_bounded_string(size_t priority) {
    std::string ret = conn->str_getter(sub);
    return (priority < conn->priority) ? '(' + ret + ')' : ret;
}

