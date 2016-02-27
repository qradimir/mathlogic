//
// Created by radimir on 22.02.16.
//

#ifndef MATHLOGIC_EXPRESSION_SCHEME_H
#define MATHLOGIC_EXPRESSION_SCHEME_H

#include <map>

#define S_IMPL(arg0, arg1)  make_operation_scheme(get_implication(), arg0, arg1)
#define S_CONJ(arg0, arg1)  make_operation_scheme(get_conjunction(), arg0, arg1)
#define S_DISJ(arg0, arg1)  make_operation_scheme(get_disjunction(), arg0, arg1)
#define S_NEG(arg0)         make_operation_scheme(get_negation(), arg0)
#define S_REF(name)         make_expression_link_ref(#name)

class expression;
class expr;
class connective;

/*
 *  Scheme
 */

class expr_scheme {
    int binds = 0;

public:
    virtual bool match(expr const& e) const = 0;
    virtual expression get_expression(std::map<std::string, expression> const &exprs) const = 0;

    inline void bind() { binds++; }
    inline void unbind() { binds--; if (binds == 0) delete(this); }

    virtual ~expr_scheme() {
    }

};

class expression_scheme {
    expr_scheme *e;

    expression_scheme() {}

public:

    expression_scheme(expr_scheme *e);
    expression_scheme(expression_scheme const &other);
    expression_scheme(expression_scheme &&other);

    ~expression_scheme();

    expression_scheme &operator=(expression_scheme const &other);

    expr_scheme const *operator->() const;

    bool match(expression const &expr) const;
    expression get_expression(std::map<std::string, expression> const &exprs) const;


    friend expression_scheme * copy_storage(size_t count, expression_scheme* storage);
};

class operation_scheme : public expr_scheme {

    connective const *conn;
    expression_scheme *storage;

protected:
    operation_scheme();
    operation_scheme(connective const *conn, expression_scheme *storage);

public:
    virtual bool match(expr const& e) const;
    virtual expression get_expression(std::map<std::string, expression> const &exprs) const;

    friend expression_scheme make_operation_scheme(connective const* conn, expression_scheme const& arg1);
    friend expression_scheme make_operation_scheme(connective const* conn, expression_scheme const& arg1, expression_scheme const& arg2);
};

expression_scheme make_operation_scheme(connective const* conn, expression_scheme const& arg1);
expression_scheme make_operation_scheme(connective const* conn, expression_scheme const& arg1, expression_scheme const& arg2);


class expression_link {
public:
    expr const* value;
    std::string name;

    expression_link(std::string const& name, expr const* value = nullptr);
};

class expression_link_ref : public expr_scheme {
    expression_link* const ref;

    expression_link_ref(expression_link *ref);

public:
    virtual bool match(expr const& e) const;
    virtual expression get_expression(std::map<std::string, expression> const &exprs) const;

    friend expression_scheme make_expression_link_ref(std::string const& name);
};

expression_scheme make_expression_link_ref(std::string const& name);

expression_link *find_expression_link(std::string const& name);

#endif //MATHLOGIC_EXPRESSION_SCHEME_H
