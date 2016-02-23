#include <iostream>
#include <vector>

#include "parser.h"
#include "axioms.h"

expression_parser eparser;
scheme_parser shparser;

variable *A;
variable *B;

std::vector<expression> exprs;

void log(expression const&  e) {
    std::cout << &e << " = " << e() << '\n';
}

expression add_test(std::string const& str) {
    expression expr = eparser.parse(str);
    exprs.insert(exprs.end(), expr);
    return expr;
}

expression gen_expr(size_t w) {
    int t = rand();
    if (w == 0) {
        return (t % 2 == 0) ? make_variable_ref(A) : make_variable_ref(B);
    }
    switch (t % 4) {
        case 0 :
            return make_operation(get_implication(), gen_expr(w - 1), gen_expr(w - 1));
        case 1 :
            return make_operation(get_disjunction(), gen_expr(w - 1), gen_expr(w - 1));
        case 2 :
            return make_operation(get_disjunction(), gen_expr(w - 1), gen_expr(w - 1));
        case 3 :
            return make_operation(get_negation(), gen_expr(w - 1));
        default:
            return make_variable_ref(A);
    }
}

void add_random_test(size_t w) {
    exprs.insert(exprs.end(), gen_expr(w));
}

void test(bool a, bool b) {
    A->value = a;
    B->value = b;

    std::cout << "A = " << a << ", B = " << b << '\n';

    for(auto it = exprs.begin(); it != exprs.end(); it++) {
        log(*it);
    }
    std::cout << '\n';
}

int main() {

    /*
     * tests
     */
    add_test("A");
    add_test("B");
    add_test("!A");
    expression impl1 = add_test("A->B");
    expression impl2 = add_test("A->B");
    add_test("A|B");
    add_test("A&B");
    add_test("A&B|!A&!B->A|B");
    add_test("A|B&A|B->A&B|A&B->(A|B)&(A|!B)");
    add_test("!(A|B&A|B->A&!(B|A|B)&B)->!(A|B)&!(!A|!B)");

    A = find_variable("A");
    B = find_variable("B");

    std::cout << (impl1 == impl2) << '\n';

    for (auto it = exprs.begin(); it != exprs.end(); ++it) {
        std::cout << *it << " is " << (*it)->to_string() << '\n';
    }

    expression_scheme implication_scheme= shparser.parse("A->B");

    for (auto it = exprs.begin(); it != exprs.end(); ++it) {
        std::cout << *it << " is " << (implication_scheme.match(*it) ? "implication" : "not implication") << '\n';
    }

    for (size_t i = 10; i < 30; i += 3) {
        add_random_test(i);
    }

    test(false, false);
    test(false, true);
    test(true, false);
    test(true, true);

    release_variables();
    release_expression_links();
    return 0;
}