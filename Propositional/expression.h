//
// Created by radimir on 02.01.16.
//

#ifndef MATHLOGIC_EXPRESSION_H
#define MATHLOGIC_EXPRESSION_H

#include <vector>
#include <memory>
#include "util.h"

#define S_IMPL(arg0, arg1)  make_operation(CONN_IMPL, arg0, arg1)
#define S_CONJ(arg0, arg1)  make_operation(CONN_CONJ, arg0, arg1)
#define S_DISJ(arg0, arg1)  make_operation(CONN_DISJ, arg0, arg1)
#define S_NEG(arg0)         make_operation(CONN_NEG, arg0)
#define S_REF(name)         make_reference(#name)

class expression;

static const ssize_t TYPE_OP = 0;
static const ssize_t TYPE_REF = 1;

static const ssize_t CONN_UNKNOWN = -1;

static const ssize_t CONN_IMPL = 0;
static const ssize_t CONN_CONJ = 1;
static const ssize_t CONN_DISJ = 2;
static const ssize_t CONN_NEG  = 3;

typedef std::pair<std::vector<expression>, bool> vsproof;
typedef evalue<bool> variable;
typedef evalue<expression> expression_link;

class expr {
protected:
    ssize_t expr_type;

public:
    expr(ssize_t type) : expr_type(type) { }

    bool operator==(expr const &other) const noexcept {
        if (expr_type != other.expr_type) {
            return false;
        }
        return hash() == other.hash() ? this->equals(other) : false;
    }

    inline ssize_t type() const noexcept {
        return expr_type;
    }

    virtual size_t hash() const noexcept = 0;
    virtual bool equals(expr const &e) const noexcept = 0;
    virtual bool evaluate(variable::holder const&) const = 0;
    virtual vsproof build_vsproof(variable::holder const&) const = 0;
    virtual bool match(expression_link::holder &, expression const &matching) const = 0;
    virtual expression substitute(expression_link::holder const &) const = 0;
    virtual std::string to_string() const = 0;

    virtual ~expr(){
    }

};

template <typename T>
void init_references(const expression &e, typename evalue<T>::holder &references);

class expression {
    std::shared_ptr<expr> e;

public:

    expression();
    expression(std::shared_ptr<expr> e);
    expression(expression const& other);
    expression(expression&& other);

    expression& operator=(expression const& other);
    expression& operator=(expression && other);

    vsproof build_vsproof(variable::holder const& vars) const;
    bool operator()(variable::holder const& vars) const;
    bool operator==(expression const & other) const;

    inline bool operator!=(expression const & other) const {
        return !(*this == other);
    }

    inline std::shared_ptr<expr> const operator*() const noexcept {
        return e;
    }

    inline std::shared_ptr<expr> const operator->() const noexcept {
        return e;
    }

    inline bool match(expression const &matching) const{
        expression_link::holder h{};
        return e->match(h, matching);
    }

    template <typename T>
    typename evalue<T>::holder values() const {
        typename evalue<T>::holder h{};
        init_references<T>(*this, h);
        return h;
    }

    friend expression* copy_storage(size_t count, expression *storage);
};

std::ostream& operator<<(std::ostream& stream, expression const& expr);

class operation : public expr {
    size_t hash_value;

protected:
    operation();
    operation(ssize_t conn, expression* storage);

public:
    const ssize_t connective_type;
    expression *const storage;

    inline expression const &get_sub(size_t i) const noexcept {
        return storage[i];
    }

    virtual size_t hash() const noexcept;
    virtual bool equals(expr const &e) const noexcept;
    virtual bool evaluate(variable::holder const&) const;
    virtual vsproof build_vsproof(variable::holder const&) const;
    virtual bool match(expression_link::holder &, expression const &) const;
    virtual expression substitute(expression_link::holder const &) const;
    virtual std::string to_string() const;

    ~operation();

    friend expression make_operation(ssize_t conn, expression *exprs);
    friend expression make_operation(ssize_t conn, expression const& arg1);
    friend expression make_operation(ssize_t conn, expression const& arg1, expression const& arg2);

};

expression make_operation(ssize_t conn, expression *exprs);

inline expression make_operation(ssize_t conn, expression const& arg1) {
    expression exprs[]{arg1};
    return make_operation(conn, exprs);
}

inline expression make_operation(ssize_t conn, expression const& arg1, expression const& arg2) {
    expression exprs[]{arg1, arg2};
    return make_operation(conn, exprs);
}

class reference : public expr {
    explicit reference(const std::string &name);

public:
    const std::string name;

    virtual size_t hash() const noexcept;
    virtual bool equals(expr const &e) const noexcept;
    virtual bool evaluate(variable::holder const&) const;
    virtual vsproof build_vsproof(variable::holder const&) const;
    virtual bool match(expression_link::holder &, expression const &) const;
    virtual expression substitute(expression_link::holder const &) const;
    virtual std::string to_string() const;

    friend expression make_reference(const std::string &name);
};

inline expression make_reference(const std::string &name) {
    return expression(std::shared_ptr<reference>(new reference(name)));
}

expression *copy_storage(size_t count, expression *storage);

template <typename T>
void push(std::vector<T> &vP, std::vector<T> const &vT) {
    vP.insert(vP.end(), vT.begin(), vT.end());
}

inline std::shared_ptr<operation> get_op(expression const &e) {
    return std::dynamic_pointer_cast<operation>(*e);
}

inline std::shared_ptr<reference> get_ref(expression const &e) {
    return std::dynamic_pointer_cast<reference>(*e);
}

inline operation const &cast_op(expr const &e) {
    return dynamic_cast<operation const &>(e);
}

inline reference const &cast_ref(expr const &e) {
    return dynamic_cast<reference const &>(e);
}

static constexpr size_t op_sub_count[]{2, 2, 2, 1};

template <typename T>
static void init_references(expression const &e, typename evalue<T>::holder &references) {
    if (e->type() == TYPE_OP) {
        std::shared_ptr<operation> op = get_op(e);
        for (size_t i = 0; i < op_sub_count[op->connective_type]; ++i) {
            init_references<T>(op->get_sub(i), references);
        }
        return;
    }
    if (e->type() == TYPE_REF) {
        references.insert(std::make_pair(get_ref(e)->name, T{}));
    }
}

#endif //MATHLOGIC_EXPRESSION_H
