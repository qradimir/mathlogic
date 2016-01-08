//
// Created by radimir on 02.01.16.
//

#include <map>
#include <vector>

#include "expression.h"

std::map<std::string, variable*> vars;
std::map<std::string, expression_link*> expr_links;
std::vector<expression const*> to_release;

expression const* start_equal_ptr;

bool expression::operator==(expression const &other) const {
    if (start_equal_ptr == nullptr) {
        start_equal_ptr = this;
    }
    bool ret;
    if (typeid(other) == typeid(expression_link_ref)) {
        ret = other.equals(*this);
    } else {
        ret = this->equals(other);
    }
    if (start_equal_ptr == this) {
        for(auto it = expr_links.begin(); it != expr_links.end(); ++it) {
            it->second->value = nullptr;
        }
        start_equal_ptr = nullptr;
    }
    return ret;
}

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
    for (size_t i = 0; i < get_sub_count(); ++i) {
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
    vars.insert(std::make_pair(name, this));
}

expression_link::expression_link(const std::string &name, expression const*value) :
        name(name),
        value(value)
{
    expr_links.insert(std::make_pair(name, this));
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


bool operation::equals(expression const &other) const {
    if (typeid(operation) != typeid(other)) {
        return false;
    }
    operation const& op = dynamic_cast<operation const&>(other);
    return conn == op.conn && storage == op.storage;
}

bool variable_ref::equals(expression const &other) const {
    if (typeid(variable_ref) != typeid(other)) {
        return false;
    }
    return ref == dynamic_cast<variable_ref const&>(other).ref;
}

bool expression_link_ref::equals(expression const &other) const {
    if (typeid(expression_link_ref) != typeid(other)) {
        if (ref->value == nullptr) {
            ref->value = &other;
            return  true;
        } else {
            return *ref->value == other;
        }
    }
    return ref == dynamic_cast<expression_link_ref const&>(other).ref;
}



void release() {
    for (auto it = vars.begin(); it != vars.end(); ++it) {
        delete(it->second);
    }
    for (auto it = expr_links.begin(); it != expr_links.end(); ++it) {
        delete(it->second);
    }
    for (auto it = to_release.begin(); it != to_release.end(); ++it) {
        delete(*it);
    }
    vars.clear();
    expr_links.clear();
    to_release.clear();
}

variable *find_variable(std::string const &name) {
    auto it = vars.find(name);
    if (it == vars.end()) {
        return nullptr;
    }
    return it->second;
}

expression_link *find_expression_link(std::string const &name) {
    auto it = expr_links.find(name);
    if (it == expr_links.end()) {
        return nullptr;
    }
    return it->second;
}

void add_to_release(expression const *expr) {
    to_release.insert(to_release.end(), expr);
}
