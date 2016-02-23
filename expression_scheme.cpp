//
// Created by radimir on 22.02.16.
//

#include <map>
#include "expression.h"
#include "expression_scheme.h"

std::map<std::string, expression_link*> expr_links;

/*
 *  expression_scheme
 */

expression_scheme::expression_scheme(expr_scheme *e)
        : e(e)
{
    if (e != nullptr) e->bind();
}

expression_scheme::expression_scheme(expression_scheme const &other)
    : expression_scheme(other.e)
{
}

expression_scheme::expression_scheme(expression_scheme &&other)
    : e(nullptr)
{
    std::swap(e, other.e);
}

expression_scheme::~expression_scheme()
{
    if (e != nullptr) e->unbind();
}

expression_scheme &expression_scheme::operator=(expression_scheme const &other) {
    if (e != nullptr) e->unbind();
    e = other.e;
    if (e != nullptr) e->bind();
    return *this;
}

bool expression_scheme::match(expression const &expr) const {
    bool ret = e->match(expr.get());
    for(auto it = expr_links.begin(); it != expr_links.end(); it++) {
        it->second->value = nullptr;
    }
    return ret;
}

expr_scheme const *expression_scheme::operator->() const {
    return e;
}

/*
 *  operation_scheme
 */

bool operation_scheme::match(expr const &e) const {
    if (typeid(operation) != typeid(e)) {
        return false;
    }
    operation const& op = dynamic_cast<operation const&>(e);
    if (conn != op.get_conn()) {
        return false;
    }
    for (size_t i = 0; i < op.get_sub_count(); ++i) {
           if (!storage[i]->match(op.get_sub(i).get())) {
               return false;
           }
    }
    return true;
}

/*
 *  expression_link
 */

expression_link::expression_link(std::string const &name, const expr *value)
    : name(name),  value(value)
{
    expr_links.insert(std::make_pair(name, this));
}

/*
 *  expression_link_ref
 */

bool expression_link_ref::match(expr const &e) const {
    if (ref->value == nullptr) {
        ref->value = &e;
        return true;
    }
    return ref->value == &e;
}

expression_link_ref::expression_link_ref(expression_link *ref)
    : ref(ref)
{
}

expression_scheme make_expression_link_ref(expression_link *ref) {
    return expression_scheme(new expression_link_ref(ref));
}

expression_link *find_expression_link(std::string const &name) {
    auto it = expr_links.find(name);
    if (it == expr_links.end()) {
        return nullptr;
    }
    return it->second;
}

expression_scheme make_operation_scheme(connective const *conn, expression_scheme const &arg1) {
    expression_scheme args[]{arg1};
    return expression_scheme(new operation_scheme(conn, args));
}

expression_scheme make_operation_scheme(connective const *conn, expression_scheme const &arg1,
                                        expression_scheme const &arg2) {
    expression_scheme args[]{arg1, arg2};
    return expression_scheme(new operation_scheme(conn, args));
}

expression_scheme *copy_storage(size_t count, expression_scheme* storage) {
    expression_scheme *t = new expression_scheme[count];
    for (size_t i = 0; i < count; ++i) {
        t[i].e = storage[i].e;
        t[i].e->bind();
    }
    return t;
}

operation_scheme::operation_scheme()
{
}

operation_scheme::operation_scheme(connective const *conn, expression_scheme *storage)
    : conn(conn), storage(copy_storage(conn->sub_count, storage))
{
}

void release_expression_links() {
    for (auto it = expr_links.begin(); it != expr_links.end(); ++it) {
        delete(it->second);
    }
    expr_links.clear();
}
