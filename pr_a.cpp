#include <iostream>

#include "expression.h"
#include "parser.h"

using namespace std;

variable A("A");
variable B("B");

void set_variables(bool a, bool b) {
    A.value = a;
    B.value = b;

    std::cout << "A = " << a << ", B = " << b << '\n';
}


void log(expression const&  e) {
    std::cout << e << " = " << e() << '\n';
}

int main() {

    /*
     * tests
     */

    expression ref_A = make_variable_ref(A);
    expression ref_B = make_variable_ref(B);

    expression e_or = make_conjunction(ref_A, ref_B);
    expression e_and = make_disjunction(ref_A, ref_B);
    expression e_impl = make_implication(ref_A, ref_B);
    expression e_not = make_negation(ref_A);

    set_variables(false, false);
    log(ref_A);
    log(ref_B);
    log(e_or);
    log(e_and);
    log(e_impl);
    log(e_not);

    std::cout << '\n';
    set_variables(false, true);
    log(ref_A);
    log(ref_B);
    log(e_or);
    log(e_and);
    log(e_impl);
    log(e_not);

    std::cout << '\n';
    set_variables(true, false);
    log(ref_A);
    log(ref_B);
    log(e_or);
    log(e_and);
    log(e_impl);
    log(e_not);

    std::cout << '\n';
    set_variables(true, true);
    log(ref_A);
    log(ref_B);
    log(e_or);
    log(e_and);
    log(e_impl);
    log(e_not);

    std::cout << '\n'
        << make_implication(e_impl, e_impl) << '\n'
        << make_implication(make_disjunction(e_or, make_negation(e_or)), make_conjunction(e_and, make_negation(e_and))) << '\n'
        << make_disjunction(make_implication(ref_A, make_implication(ref_A, ref_B)), make_disjunction( make_conjunction(make_conjunction(ref_A, ref_B), ref_A), make_negation(ref_A)));

    return 0;
}