//
// Created by radimir on 02.01.16.
//

#ifndef MATHLOGIC_EXPRESSION_H
#define MATHLOGIC_EXPRESSION_H

#include <functional>
#include <assert.h>

class _expr {
    int binds = 0;
protected:
    virtual bool equals(_expr const& other) const = 0;

public:
    virtual bool operator()() const = 0;
    virtual std::string to_string() const = 0;
    virtual size_t get_priority() const = 0;

    inline void bind() { binds++; }
    inline void unbind() { binds--; if (binds == 0) delete(this); }

    bool operator==(_expr const& other) const;

    std::string to_bounded_string(int priority) const {
        return (priority < this->get_priority()) ? '(' + this->to_string() + ')' : this->to_string();
    }

    virtual ~_expr() {
    }
};

class expression {
    _expr* expr;

    expression() {}

public:

    expression(_expr* expr);
    expression(expression const& other);

     ~expression();

    expression& operator=(expression const& other);

    bool operator()() const;
    bool operator==(expression const& other) const;

    _expr const* operator->() const;

    friend expression* copy_storage(size_t count, expression* storage);
};

std::ostream& operator<<(std::ostream& stream, expression const& expr);

class connective {
public:

    size_t sub_count;
    size_t priority;
    std::function<bool(bool*)> evaluator;
    std::function<std::string(expression const*)> str_getter;

    connective(std::function<bool(bool*)> evaluator, std::function<std::string(expression const*)> str_getter,
               size_t priority, size_t sub_count);

};

class operation : public _expr {

    connective const* conn;

    expression* storage;

protected:
    virtual bool equals(_expr const &other) const;

    operation();
    operation(connective const *conn, expression* storage);

public:
    operation(operation const& other);

    ~operation();

    inline size_t get_sub_count() const;

    inline expression const& get_sub(size_t i) const;

    virtual bool operator()() const;
    virtual std::string to_string() const;
    virtual size_t get_priority() const;

    friend expression make_operation(connective const* conn, expression const& arg1);
    friend expression make_operation(connective const* conn, expression const& arg1, expression const& arg2);
};

expression make_operation(connective const* conn, expression const& arg1);
expression make_operation(connective const* conn, expression const& arg1, expression const& arg2);

inline size_t operation::get_sub_count() const {
    return conn->sub_count;
}

inline expression const& operation::get_sub(size_t i) const {
    return storage[i];
}

class variable {
public:
    bool value;
    std::string name;

    variable(std::string const& name, bool value = false);

};

class expression_link {
public:
    _expr const* value;
    std::string name;

    inline bool operator()() {
        assert(value != nullptr);

        return value->operator()();
    }

    expression_link(std::string const& name, _expr const* value = nullptr);
};

class variable_ref : public _expr {

    variable* ref;

    variable_ref(variable* ref);

protected:
    virtual bool equals(_expr const &other) const;

public:
    inline variable& operator->() {
        return *ref;
    }

    virtual bool operator()() const;
    virtual std::string to_string() const;
    virtual size_t get_priority() const;

    friend expression make_variable_ref(variable* ref);
};

expression make_variable_ref(variable* ref);

class expression_link_ref : public _expr {

    expression_link* const ref;

    expression_link_ref(expression_link* ref);

protected:
    virtual bool equals(_expr const& other) const;

public:
    inline expression_link const& operator->() {
        return *ref;
    }

    virtual bool operator()() const;
    virtual std::string to_string() const;
    virtual size_t get_priority() const;

    friend expression make_expression_link_ref(expression_link* ref);
};

expression make_expression_link_ref(expression_link* ref);

static connective negation(
        [] (bool* args) -> bool { return !args[0]; },
        [] (expression const* storage) -> std::string { return "!" + storage[0]->to_bounded_string(1); },
        1, 1
);

static connective conjunction(
        [] (bool* args) -> bool { return args[0] & args[1]; },
        [] (expression const* storage) -> std::string { return storage[0]->to_bounded_string(2) + "&" + storage[1]->to_bounded_string(2); },
        2, 2
);

static connective disjunction(
        [] (bool* args) -> bool { return args[0] | args[1]; },
        [] (expression const* storage) -> std::string { return storage[0]->to_bounded_string(3) + "|" + storage[1]->to_bounded_string(3); },
        3, 2
);

static connective implication(
        [] (bool* args) -> bool { return !args[0] | args[1]; },
        [] (expression const* storage) -> std::string { return storage[0]->to_bounded_string(3) + "->" + storage[1]->to_bounded_string(4); },
        4, 2
);

variable* find_variable(std::string const& name);
expression_link* find_expression_link(std::string const& name);


void release();

#endif //MATHLOGIC_EXPRESSION_H
