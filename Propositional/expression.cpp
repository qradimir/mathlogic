//
// Created by radimir on 02.01.16.
//

#include <map>
#include <vector>

#include "util.h"
#include "expression.h"
#include "axioms.h"

/*
 *  expression
 */

expression::expression() { }

expression::expression(std::shared_ptr<expr> e)
    : e(e)
{
}

expression::expression(const expression &e)
    : expression(e.e)
{
}

expression::expression(expression &&e)
{
    std::swap(this->e, e.e);
}

expression &expression::operator=(expression const &other) {
    e = other.e;
    return *this;
}

expression &expression::operator=(expression &&other) {
    std::swap(e, other.e);
    return *this;
}

bool expression::operator()(variable::holder const& vars) const {
    return e->evaluate(vars);
}

vsproof expression::build_vsproof(variable::holder const& vars) const {
    return e->build_vsproof(vars);
}

bool expression::operator==(expression const & other) const {
    if (e == other.e)
        return true;
    return *e == *other.e;
}

std::ostream &operator<<(std::ostream &stream, expression const &expr) {
    return stream << expr->to_string();
}

expression* copy_storage(size_t count, expression *storage) {
    expression *t = new expression[count];
    for (size_t i = 0; i < count; ++i) {
        t[i] = storage[i];
    }
    return t;
}

/*
 *  operation
 */

operation::operation()
    : operation(CONN_UNKNOWN, new expression[0])
{
}

#define HASHED(value) hash_value = 31 * hash_value + (size_t)(value);

operation::operation(ssize_t conn, expression *storage)
    : expr(TYPE_OP), connective_type(conn), storage(storage)
{
    hash_value = 0;
    HASHED(connective_type);
    for (size_t i = 0; i < op_sub_count[connective_type]; ++i) {
        HASHED(storage[i]->hash());
    }
}

bool operation::evaluate(variable::holder const &vars) const {
    static std::function<bool(bool*)> evaluator[]{
            [] (bool* args) -> bool { return !args[0] | args[1]; },
            [] (bool* args) -> bool { return  args[0] & args[1]; },
            [] (bool* args) -> bool { return  args[0] | args[1]; },
            [] (bool* args) -> bool { return !args[0]; },

    };
    size_t s = op_sub_count[connective_type];
    bool values[s];
    for (size_t i = 0; i < s; ++i) {
        values[i] = storage[i]->evaluate(vars);
    }
    return evaluator[connective_type](values);
}

vsproof operation::build_vsproof(variable::holder const &vars) const {
    static std::function<vsproof(expression*, variable::holder const&)> proofer[]{
            [] (expression const *s, variable::holder const &v) -> vsproof{
                vsproof p = s[0]->build_vsproof(v), t = s[1]->build_vsproof(v);
                expression_link::holder m;
                m["A"] = s[0];
                m["B"] = s[1];
                push(p.first, t.first);
                push(p.first, get_sub_list(get_impl_vsproof(p.second, t.second), m));
                return std::make_pair(p.first, !p.second | t.second);
            },
            [] (expression const *s, variable::holder const &v) -> vsproof{
                vsproof p = s[0]->build_vsproof(v), t = s[1]->build_vsproof(v);
                expression_link::holder m;
                m["A"] = s[0];
                m["B"] = s[1];
                push(p.first, t.first);
                push(p.first, get_sub_list(get_conj_vsproof(p.second, t.second), m));
                return std::make_pair(p.first, p.second & t.second );
            },
            [] (expression const *s, variable::holder const &v) -> vsproof {
                vsproof p = s[0]->build_vsproof(v), t = s[1]->build_vsproof(v);
                expression_link::holder m;
                m["A"] = s[0];
                m["B"] = s[1];
                push(p.first, t.first);
                push(p.first, get_sub_list(get_disj_vsproof(p.second, t.second), m));
                return std::make_pair(p.first, p.second | t.second);
            },
            [] (expression const *s, variable::holder const &v) -> vsproof {
                auto p = s[0]->build_vsproof(v);
                expression_link::holder m;
                m["A"] = s[0];
                push(p.first, get_sub_list(get_not_vsproof(p.second), m));
                return std::make_pair(p.first, !p.second);
            }
    };
    return proofer[connective_type](storage, vars);
}

std::string operation::to_string() const {
    static std::function<std::string(expression*)> op_name[]{
            [](expression *e) -> std::string { return "(" + e[0]->to_string() + ")->(" + e[1]->to_string() + ")"; },
            [](expression *e) -> std::string { return "(" + e[0]->to_string() + ")&("  + e[1]->to_string() + ")"; },
            [](expression *e) -> std::string { return "(" + e[0]->to_string() + ")|("  + e[1]->to_string() + ")"; },
            [](expression *e) -> std::string { return "!(" + e[0]->to_string() + ")"; }
    };
    return op_name[connective_type](storage);
}


size_t operation::hash() const noexcept {
    return hash_value;
}


bool operation::equals(expr const &e) const noexcept {
    operation const &op  = cast_op(e);
    if (connective_type != op.connective_type) {
        return false;
    }
    for (size_t i = 0; i < op_sub_count[connective_type]; ++i) {
        if (storage[i] != op.storage[i]) {
            return false;
        }
    }
    return true;
}

bool operation::match(expression_link::holder &links, expression const &matching) const {
    if (matching->type() != TYPE_OP) {
        return false;
    }
    std::shared_ptr<operation> op = get_op(matching);
    if (connective_type != op->connective_type) {
        return false;
    }
    for (size_t i = 0; i < op_sub_count[connective_type]; ++i) {
        if (!storage[i]->match(links, op->storage[i])) {
            return false;
        }
    }
    return true;
}

expression operation::substitute(expression_link::holder const &holder) const {
    expression ret[op_sub_count[connective_type]];
    for (size_t i = 0; i < op_sub_count[connective_type]; ++i) {
        ret[i] = storage[i]->substitute(holder);
    }
    return make_operation(connective_type, ret);
}

operation::~operation() {
    delete[](storage);
}


expression make_operation(ssize_t conn, expression *exprs) {
    return expression(std::shared_ptr<operation>(new operation(conn, copy_storage(op_sub_count[conn], exprs))));
}

/*
 *  reference
 */

reference::reference(std::string const &name)
        : expr(TYPE_REF), name(name)
{
}

bool reference::evaluate(variable::holder const &vars) const {
    auto var = vars.find(name);
    if (var == vars.end()) {
        throw variable::no_value{vars, name};
    }
    return var->second;
}

vsproof reference::build_vsproof(variable::holder const &vars) const {
    bool val = evaluate(vars);
    expression e = make_reference(name);
    return std::make_pair(std::vector<expression>{expression(val ? e : S_NEG(e))}, val);
}

std::string reference::to_string() const {
    return name;
}

size_t reference::hash() const noexcept {
    static std::hash<std::string> str_hash{};
    return str_hash(name);
}

bool reference::equals(expr const &e) const noexcept {
    return cast_ref(e).name == name;
}

bool reference::match(expression_link::holder & links, expression const &matching) const {
    auto ptr = links.find(name);
    if (ptr == links.end()) {
        links.insert(std::make_pair(name, matching));
        return true;
    }
    return ptr->second == matching;
}

expression reference::substitute(expression_link::holder const &holder) const {
    auto ptr = holder.find(name);
    if (ptr == holder.end()) {
        throw expression_link::no_value{holder, name};
    }
    return ptr->second;
}