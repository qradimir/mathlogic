//
// Created by radimir on 02.01.16.
//

#ifndef MATHLOGIC_EXPRESSION_H
#define MATHLOGIC_EXPRESSION_H

#include <functional>
#include <assert.h>

/*
 *  Expression
 */

class expr {
    int binds = 0;
protected:
    virtual bool equals(expr const& other) const = 0;

public:
    virtual bool operator()() const = 0;
    virtual std::string to_string() const = 0;
    virtual size_t get_priority() const = 0;
    virtual size_t hash() const = 0;

    inline void bind() { binds++; }
    inline void unbind() { binds--; if (binds == 0) delete(this); }

    bool operator==(expr const& other) const;

    std::string to_bounded_string(int priority) const {
        return (priority < this->get_priority()) ? '(' + this->to_string() + ')' : this->to_string();
    }

    virtual ~expr() {
    }
};

class expression {
    expr * e;

    expression() {}

public:

    expression(expr * e);
    expression(expression const& other);
    expression(expression&& other);

     ~expression();

    expression& operator=(expression const& other);

    bool operator()() const;
    bool operator==(expression const& other) const;
    bool operator!=(expression const& other) const;

    inline expr const* operator->() const {
        return e;
    }

    inline expr const& get() const {
        return *e;
    }

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

class operation : public expr {

    connective const* conn;

    expression* storage;

    size_t hash_value;

protected:
    virtual bool equals(expr const &other) const;

    void count_hash();

    operation();
    operation(connective const *conn, expression* storage);

public:
    operation(operation const& other);

    ~operation();

    inline connective const *get_conn() const;
    inline size_t get_sub_count() const;
    inline expression const &get_sub(size_t i) const;

    virtual bool operator()() const;
    virtual std::string to_string() const;
    virtual size_t get_priority() const;


    virtual size_t hash() const;

    friend expression make_operation(connective const* conn, expression const& arg1);
    friend expression make_operation(connective const* conn, expression const& arg1, expression const& arg2);
};

expression make_operation(connective const* conn, expression const& arg1);
expression make_operation(connective const* conn, expression const& arg1, expression const& arg2);

inline connective const *operation::get_conn() const {
    return conn;
}

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

class variable_ref : public expr {

    variable* ref;

    variable_ref(variable* ref);

protected:
    virtual bool equals(expr const &other) const;

public:
    inline variable& operator->() {
        return *ref;
    }

    virtual bool operator()() const;
    virtual std::string to_string() const;
    virtual size_t get_priority() const;


    virtual size_t hash() const;

    friend expression make_variable_ref(variable* ref);
};

expression make_variable_ref(variable* ref);

connective* get_implication();
connective* get_disjunction();
connective* get_conjunction();
connective* get_negation();

variable* find_variable(std::string const& name);


void release_variables();

#endif //MATHLOGIC_EXPRESSION_H
