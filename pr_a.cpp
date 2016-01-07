#include <iostream>
#include <vector>

#include "parser.h"


parser p;

variable *A;
variable *B;

std::vector<expression const*> exprs;

void log(expression const*  e) {
    std::cout << e << " = " << e->operator()() << '\n';
}

void add_test(std::string const& str) {
    exprs.insert(exprs.end(), p.parse(str));
}

expression const* gen_expr(size_t w) {
    int t = rand();
    if (w == 0) {
        return (t % 2 == 0) ? new variable_ref(A) : new variable_ref(B);
    }
    switch (t % 4) {
        case 0 :
            return new operation(&implication, expression_storage(gen_expr(w - 1), gen_expr(w - 1)));
        case 1 :
            return new operation(&disjunction, expression_storage(gen_expr(w - 1), gen_expr(w - 1)));
        case 2 :
            return new operation(&conjunction, expression_storage(gen_expr(w - 1), gen_expr(w - 1)));
        case 3 :
            return new operation(&negation, expression_storage(gen_expr(w - 1)));
        default:
            return new variable_ref(A);
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
    add_test("A->B");
    add_test("A|B");
    add_test("A&B");
    add_test("A&B|!A&!B->A|B");
    add_test("A|B&A|B->A&B|A&B->(A|B)&(A|!B)");
    add_test("!(A|B&A|B->A&!(B|A)&B)->!(A|B)&!(!A|!B)");

    A = p.get_variable("A");
    B = p.get_variable("B");

    for (int i = 10; i < 30; i += 3) {
        add_random_test(i);
    }

    for (auto it = exprs.begin(); it != exprs.end(); ++it) {
        std::cout << *it << " is " << (*it)->to_string() << '\n';
    }

    test(false, false);
    test(false, true);
    test(true, false);
    test(true, true);

    for (auto it = exprs.begin(); it != exprs.end(); ++it) {
        delete(*it);
    }

    return 0;
}