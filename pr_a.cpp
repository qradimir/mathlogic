#include <iostream>
#include <vector>

#include "expression.h"
#include "parser.h"


parser p;

variable *A;
variable *B;

std::vector<expression> exprs;

void log(expression const&  e) {
    std::cout << e << " = " << e() << '\n';
}

void add_test(std::string const& str) {
    exprs.insert(exprs.end(), p.parse(str));
}

expression gen_expr(size_t w) {
    int t = rand();
    if (w == 0) {
        return (t % 2 == 0) ? make_variable_ref(*A) : make_variable_ref(*B);
    }
    switch (t % 4) {
        case 0 :
            return make_implication(gen_expr(w - 1), gen_expr(w - 1));
        case 1 :
            return make_disjunction(gen_expr(w - 1), gen_expr(w - 1));
        case 2 :
            return make_conjunction(gen_expr(w - 1), gen_expr(w - 1));
        case 3 :
            return make_negation(gen_expr(w - 1));
        default:
            return make_variable_ref(*A);
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

    add_random_test(6);
    add_random_test(8);
    add_random_test(10);
    add_random_test(12);
    add_random_test(14);
    add_random_test(16);
    add_random_test(18);
    add_random_test(20);

    //TODO : copy works too slow
    //add_random_test(22);

    test(false, false);
    test(false, true);
    test(true, false);
    test(true, true);

    return 0;
}