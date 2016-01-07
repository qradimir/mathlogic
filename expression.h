//
// Created by radimir on 02.01.16.
//

#ifndef MATHLOGIC_EXPRESSION_H
#define MATHLOGIC_EXPRESSION_H

#include <functional>
#include <assert.h>

#include "expression_storage.h"

class connective;

class expression {
public:
    virtual bool operator()() const = 0;
    virtual std::string to_string() const = 0;
    virtual size_t get_priority() const = 0;

    std::string to_bounded_string(int priority) const {
        return (priority < get_priority()) ? '(' + this->to_string() + ')' : this->to_string();
    }

    virtual ~expression() {
    }
};

class operation : public expression {

    connective const* conn;

    expression_storage storage;

public:

    operation();
    operation(connective const *conn, expression_storage const &storage);

    operation(operation const& other);

    inline size_t get_sub_count() const;

    inline const expression * get_sub(size_t i) const;

    void operator=(operation const& other);

    virtual bool operator()() const;
    virtual std::string to_string() const;
    virtual size_t get_priority() const;
};

inline size_t operation::get_sub_count() const {
    return storage.size();
}

inline const expression * operation::get_sub(size_t i) const {
    return storage.get(i);
}

class connective {
public:
    size_t priority;
    std::function<bool(bool*)> evaluator;
    std::function<std::string(expression_storage const&)> str_getter;

    connective(std::function<bool(bool*)> evaluator, std::function<std::string(expression_storage const&)> str_getter, size_t priority);

};

class variable {
public:
    bool value;
    std::string name;

    variable(std::string const& name, bool value = false);

};

class expression_link {
public:
    expression * value;
    std::string name;

    inline bool operator()() {
        assert(value != nullptr);

        return value->operator()();
    }

    expression_link(std::string const& name, expression * value = nullptr);
};

class variable_ref : public expression {

    variable* const ref;

public:
    inline variable& operator->() {
        return *ref;
    }

    virtual bool operator()() const;
    virtual std::string to_string() const;
    virtual size_t get_priority() const;

    variable_ref(variable* ref);
};

class expression_link_ref : public expression {

    expression_link* const ref;

public:
    inline expression_link const& operator->() {
        return *ref;
    }

    virtual bool operator()() const;
    virtual std::string to_string() const;
    virtual size_t get_priority() const;

    expression_link_ref(expression_link* ref);
};

static connective negation(
        [] (bool* args) -> bool { return !args[0]; },
        [] (expression_storage const& storage) -> std::string { return "!" + storage[0]->to_bounded_string(1); },
        1
);

static connective conjunction(
        [] (bool* args) -> bool { return args[0] & args[1]; },
        [] (expression_storage const& storage) -> std::string { return storage[0]->to_bounded_string(2) + "&" + storage[1]->to_bounded_string(2); },
        2
);

static connective disjunction(
        [] (bool* args) -> bool { return args[0] | args[1]; },
        [] (expression_storage const& storage) -> std::string { return storage[0]->to_bounded_string(3) + "|" + storage[1]->to_bounded_string(3); },
        3
);

static connective implication(
        [] (bool* args) -> bool { return !args[0] | args[1]; },
        [] (expression_storage const& storage) -> std::string { return storage[0]->to_bounded_string(3) + "->" + storage[1]->to_bounded_string(4); },
        4
);

#endif //MATHLOGIC_EXPRESSION_H
