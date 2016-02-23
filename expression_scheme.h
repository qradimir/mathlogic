//
// Created by radimir on 22.02.16.
//

#ifndef MATHLOGIC_EXPRESSION_SCHEME_H
#define MATHLOGIC_EXPRESSION_SCHEME_H

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

    friend expression_scheme make_expression_link_ref(expression_link* ref);
};

expression_scheme make_expression_link_ref(expression_link* ref);
expression_link *find_expression_link(std::string const& name);

void release_expression_links();

#endif //MATHLOGIC_EXPRESSION_SCHEME_H
