//
// Created by radimir on 08.01.16.
//

#ifndef MATHLOGIC_AXIOMAS_H
#define MATHLOGIC_AXIOMAS_H

#include "expression.h"
#include "expression_scheme.h"
#include "parser.h"
#include <vector>
#include <string>

static expression_scheme axioms[]{
//X->Y->X
    S_IMPL(S_REF(X),S_IMPL(S_REF(Y),S_REF(X))),
//(X->Y)->(X->Y->Z)->(X->Z)"
    S_IMPL(S_IMPL(S_REF(X),S_REF(Y)),S_IMPL(S_IMPL(S_REF(X),S_IMPL(S_REF(Y),S_REF(Z))),S_IMPL(S_REF(X),S_REF(Z)))),
//X->Y->X&Y
    S_IMPL(S_REF(X),S_IMPL(S_REF(Y),S_CONJ(S_REF(X),S_REF(Y)))),
//X&Y->X
    S_IMPL(S_CONJ(S_REF(X),S_REF(Y)),S_REF(X)),
//X&Y->Y
    S_IMPL(S_CONJ(S_REF(X),S_REF(Y)),S_REF(Y)),
//X->X|Y
    S_IMPL(S_REF(X),S_DISJ(S_REF(X),S_REF(Y))),
//Y->X|Y
    S_IMPL(S_REF(Y),S_DISJ(S_REF(X),S_REF(Y))),
//(X->Z)->(Y->Z)->(X|Y->Z)
    S_IMPL(S_IMPL(S_REF(X),S_REF(Z)),S_IMPL(S_IMPL(S_REF(Y),S_REF(Z)),S_IMPL(S_DISJ(S_REF(X),S_REF(Y)),S_REF(Z)))),
//(X->Y)->(X->!Y)->!X
    S_IMPL(S_IMPL(S_REF(X),S_REF(Y)),S_IMPL(S_IMPL(S_REF(X),S_NEG(S_REF(Y))),S_NEG(S_REF(X)))),
//!!X->X
    S_IMPL(S_NEG(S_NEG(S_REF(X))),S_REF(X))
};
static expression_scheme a_impl_a_proof[]{
//A->A->A
    S_IMPL(S_REF(A),S_IMPL(S_REF(A),S_REF(A))),
//(A->A->A)->(A->(A->A)->A)->(A->A)
    S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_REF(A),S_REF(A))),S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_REF(A),S_REF(A)),S_REF(A))),S_IMPL(S_REF(A),S_REF(A)))),
//(A->(A->A)->A)->(A->A)
    S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_REF(A),S_REF(A)),S_REF(A))),S_IMPL(S_REF(A),S_REF(A))),
//A->(A->A)->A
    S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_REF(A),S_REF(A)),S_REF(A))),
//A->A
    S_IMPL(S_REF(A),S_REF(A))
};

//  Y->X
static expression_scheme mp_scheme_0 = S_IMPL(S_REF(Y),S_REF(X));

//  (X->Y->Z)->(X->Z)
static expression_scheme mp_scheme_1 = S_IMPL(S_IMPL(S_REF(X),S_IMPL(S_REF(Y),S_REF(Z))),S_IMPL(S_REF(X),S_REF(Y)));

//  X->Z
static expression_scheme mp_mp_scheme_1 = S_IMPL(S_REF(X), S_REF(Z));


std::vector<expression> get_a_impl_a_proof(expression const& sub);
int is_axiom(expression const& expr);


#endif //MATHLOGIC_AXIOMAS_H
