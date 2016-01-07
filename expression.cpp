//
// Created by radimir on 02.01.16.
//

#include "expression.h"


operation::operation()
{
}

operation::operation(const operation &other)
    : conn(other.conn), storage(other.storage)
{
}

operation::operation(connective const *conn, expression_storage const &storage)
    : conn(conn), storage(storage)
{
}

void operation::operator=(operation const &other) {
    conn = other.conn;
    storage = other.storage;
}


bool operation::operator()() const {
    bool values[get_sub_count()];
    for (int i = 0; i < get_sub_count(); ++i) {
        values[i] = get_sub(i)->operator()();
    }
    return conn->evaluator(values);
}

std::string operation::to_string() const {
    return conn->str_getter(storage);
}

size_t operation::get_priority() const {
    return conn->priority;
}

variable::variable(std::string const& name, bool value) :
        name(name),
        value(value)
{
}

expression_link::expression_link(const std::string &name, expression *value) :
        name(name),
        value(value)
{
}

connective::connective(std::function<bool(bool*)> evaluator, std::function<std::string(expression_storage const&)> str_getter, size_t priority)
        : evaluator(evaluator), str_getter(str_getter), priority(priority) {
}


variable_ref::variable_ref(variable *ref)
    : ref(ref)
{
}

bool variable_ref::operator()() const{
    return ref->value;
}

std::string variable_ref::to_string() const{
    return ref->name;
}

size_t variable_ref::get_priority() const{
    return 0;
}

expression_link_ref::expression_link_ref(expression_link *ref)
    : ref(ref)
{
}

bool expression_link_ref::operator()() const{
    return ref->operator()();
}

std::string expression_link_ref::to_string() const{
    return ref->name;
}

size_t expression_link_ref::get_priority() const{
    return 0;
}
