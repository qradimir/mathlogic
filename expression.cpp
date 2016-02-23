//
// Created by radimir on 02.01.16.
//

#include <map>
#include <vector>

#include "expression.h"

#define HASHED(value) hash_value = 31 * hash_value + (size_t)(value)

std::map<std::string, variable*> vars;

/*
 *  expression
 */

expression::expression(expr *e)
    : e(e)
{
    if (e != nullptr) e->bind();
}

expression::expression(expression const &other)
    : expression(other.e)
{
}

expression::expression(expression &&other)
        : e(nullptr)
{
    std::swap(e, other.e);
}

expression::~expression() {
    if (e != nullptr) e->unbind();
}

expression &expression::operator=(expression const &other) {
    if (e != nullptr) e->unbind();
    e = other.e;
    if (e != nullptr) e->bind();
    return *this;
}

bool expression::operator()() const {
    assert(e != nullptr);
    return (*e)();
}

bool expression::operator==(expression const& other) const {
    if (e == other.e) {
        return true;
    }
    assert(e != nullptr && other.e != nullptr);
    return *e == *other.e;
}


bool expression::operator!=(expression const &other) const {
    return !(*this == other);
}

/*
 *  expr
 */

bool expr::operator==(expr const &other) const {
    if (hash() != other.hash()) {
        return false;
    }
    return this->equals(other);
}

/*
 *  operation
 */

operation::operation()
{
}

operation::operation(const operation &other)
    : operation(other.conn, other.storage)
{
}

operation::~operation() {
    delete[](storage);
}


expression make_operation(connective const *conn, expression const &arg1) {
    expression exprs[]{arg1};
    expr *t = new operation(conn, exprs);
    return expression(t);
}

expression make_operation(connective const *conn, expression const &arg1, expression const &arg2) {
    expression exprs[]{arg1, arg2};
    expr *t = new operation(conn, exprs);
    return expression(t);
}

expression* copy_storage(size_t count, expression* storage) {
    expression *t = new expression[count];
    for (size_t i = 0; i < count; ++i) {
        t[i].e = storage[i].e;
        t[i].e->bind();
    }
    return t;
}

operation::operation(connective const *conn, expression *storage)
    : conn(conn), storage(copy_storage(conn->sub_count, storage))
{
    count_hash();
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

bool operation::equals(expr const &other) const {
    if (typeid(operation) != typeid(other)) {
        return false;
    }
    operation const& op = dynamic_cast<operation const&>(other);
    if (conn != op.conn) {
        return false;
    }
    for (int i = 0; i < conn->sub_count; ++i) {
        if (storage[i] != op.storage[i]) {
            return false;
        }
    }
    return true;
}

size_t operation::hash() const {
    return hash_value;
}

void operation::count_hash() {
    hash_value = 0;
    HASHED(conn);
    for (int i = 0; i < conn->sub_count; ++i) {
        HASHED(storage[i]->hash());
    }
}

variable::variable(std::string const& name, bool value) :
        name(name),
        value(value)
{
    vars.insert(std::make_pair(name, this));
}

/*
 *  variable_ref
 */

variable_ref::variable_ref(variable *ref)
    : ref(ref)
{
}

expression make_variable_ref(variable *ref) {
    return expression(new variable_ref(ref));
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

size_t variable_ref::hash() const {
    return (size_t) ref;
}

bool variable_ref::equals(expr const &other) const {
    if (typeid(variable_ref) != typeid(other)) {
        return false;
    }
    return ref == dynamic_cast<variable_ref const&>(other).ref;
}



void release_variables() {
    for (auto it = vars.begin(); it != vars.end(); ++it) {
        delete(it->second);
    }
    vars.clear();
}

variable *find_variable(std::string const &name) {
    auto it = vars.find(name);
    if (it == vars.end()) {
        return nullptr;
    }
    return it->second;
}



/*
 *  connective
 */

connective::connective(std::function<bool(bool *)> evaluator, std::function<std::string(expression const *)> str_getter,
                       size_t priority, size_t sub_count)
        : evaluator(evaluator), str_getter(str_getter), priority(priority), sub_count(sub_count)
{
}

connective negation(
        [] (bool* args) -> bool { return !args[0]; },
        [] (expression const* storage) -> std::string { return "!" + storage[0]->to_bounded_string(1); },
        1, 1
);

connective conjunction(
        [] (bool* args) -> bool { return args[0] & args[1]; },
        [] (expression const* storage) -> std::string { return storage[0]->to_bounded_string(2) + "&" + storage[1]->to_bounded_string(2); },
        2, 2
);

connective disjunction(
        [] (bool* args) -> bool { return args[0] | args[1]; },
        [] (expression const* storage) -> std::string { return storage[0]->to_bounded_string(3) + "|" + storage[1]->to_bounded_string(3); },
        3, 2
);

connective implication(
        [] (bool* args) -> bool { return !args[0] | args[1]; },
        [] (expression const* storage) -> std::string { return storage[0]->to_bounded_string(3) + "->" + storage[1]->to_bounded_string(4); },
        4, 2
);

connective *get_implication() {
    return &implication;
}

connective *get_disjunction() {
    return &disjunction;
}

connective *get_conjunction() {
    return &conjunction;
}

connective *get_negation() {
    return &negation;
}

std::ostream &operator<<(std::ostream &stream, expression const &expr) {
    return stream << expr->to_string();
}
