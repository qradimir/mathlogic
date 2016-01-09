//
// Created by radimir on 02.01.16.
//

#include <map>
#include <vector>

#include "expression.h"

std::map<std::string, variable*> vars;
std::map<std::string, expression_link*> expr_links;


expression::expression(_expr *expr)
    : expr(expr)
{
    expr->bind();
}

expression::expression(expression const &other)
    : expression(other.expr)
{
}

expression::~expression() {
    expr->unbind();
}

expression &expression::operator=(expression const &other) {
    if (expr != nullptr) expr->unbind();
    expr = other.expr;
    if (expr != nullptr) expr->bind();
    return *this;
}

bool expression::operator()() const {
    assert(expr != nullptr);
    return (*expr)();
}

bool expression::operator==(expression const& other) const {
    if (expr == other.expr) {
        return true;
    }
    assert(expr != nullptr && other.expr != nullptr);
    return *expr == *other.expr;
}


bool expression::operator!=(expression const &other) const {
    return !(*this == other);
}

_expr const *expression::operator->() const {
    return expr;
}

_expr const* start_equal_ptr;

bool _expr::operator==(_expr const &other) const {
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

operation::~operation() {
    delete[](storage);
}


expression make_operation(connective const *conn, expression const &arg1) {
    expression exprs[]{arg1};
    _expr* _= new operation(conn, exprs);
    return expression(_);
}

expression make_operation(connective const *conn, expression const &arg1, expression const &arg2) {
    expression exprs[]{arg1, arg2};
    _expr* _= new operation(conn, exprs);
    return expression(_);
}

expression* copy_storage(size_t count, expression* storage) {
    expression* _ = new expression[count];
    for (size_t i = 0; i < count; ++i) {
        _[i].expr = storage[i].expr;
        _[i].expr->bind();
    }
    return _;
}

operation::operation(connective const *conn, expression *storage)
    : conn(conn), storage(copy_storage(conn->sub_count, storage))
{
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

expression_link::expression_link(const std::string &name, _expr const*value) :
        name(name),
        value(value)
{
    expr_links.insert(std::make_pair(name, this));
}


connective::connective(std::function<bool(bool *)> evaluator, std::function<std::string(expression const *)> str_getter,
                       size_t priority, size_t sub_count)
    : evaluator(evaluator), str_getter(str_getter), priority(priority), sub_count(sub_count)
{
}

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

expression_link_ref::expression_link_ref(expression_link *ref)
    : ref(ref)
{
}


expression make_expression_link_ref(expression_link *ref) {
    return expression(new expression_link_ref(ref));
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


bool operation::equals(_expr const &other) const {
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

bool variable_ref::equals(_expr const &other) const {
    if (typeid(variable_ref) != typeid(other)) {
        return false;
    }
    return ref == dynamic_cast<variable_ref const&>(other).ref;
}

bool expression_link_ref::equals(_expr const &other) const {
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
    vars.clear();
    expr_links.clear();
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
