//
// Created by radimir on 09.01.16.
//

#include <iostream>
#include "axioms.h"

typedef std::vector<expression_scheme> sh_list;

sh_list AXIOM{
//X->Y->X
        S_IMPL(S_REF(X),S_IMPL(S_REF(Y),S_REF(X))),
//(X->Y)->(X->Y->Z)->(X->Z)
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
sh_list AX_DEDUC{
//X
        S_REF(X),
//X->Y->X
        S_IMPL(S_REF(X),S_IMPL(S_REF(Y),S_REF(X))),
//Y->X
        S_IMPL(S_REF(Y), S_REF(X)),
};
sh_list AA_DEDUC{
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
sh_list MP_DEDUC{
//(X->Y)->(X->Y->Z)->(X->Z)
        S_IMPL(S_IMPL(S_REF(X),S_REF(Y)),S_IMPL(S_IMPL(S_REF(X),S_IMPL(S_REF(Y),S_REF(Z))),S_IMPL(S_REF(X),S_REF(Z)))),
//(X->Y->Z)->(X->Z)
        S_IMPL(S_IMPL(S_REF(X), S_IMPL(S_REF(Y), S_REF(Z))), S_IMPL(S_REF(X), S_REF(Z))),
//X->Z
        S_IMPL(S_REF(X), S_REF(Z))
};

/*
 *  implication
 */

sh_list AB_IMPL{
//B->A->B
        S_IMPL(S_REF(B),S_IMPL(S_REF(A),S_REF(B))),
//A->B
        S_IMPL(S_REF(A),S_REF(B))
};
sh_list ANB_NIMPL{
//((A->B)->B)->((A->B)->!B)->!(A->B)
        S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_REF(B)),S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_NEG(S_REF(B))),S_NEG(S_IMPL(S_REF(A),S_REF(B))))),
//((A->B)->A)->((A->B)->(A->B))->((A->B)->B)
        S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_REF(A)),S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_REF(A),S_REF(B))),S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_REF(B)))),
//A->(A->B)->A
        S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_REF(A))),
//(A->B)->A
        S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_REF(A)),
//((A->B)->(A->B))->((A->B)->B)
        S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_REF(A),S_REF(B))),S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_REF(B))),
//((A->B)->(A->B)->(A->B))
        S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_REF(A),S_REF(B)))),
//((A->B)->(A->B)->(A->B))->((A->B)->((A->B)->(A->B))->(A->B))->((A->B)->(A->B))
        S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_REF(A),S_REF(B)))),S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_REF(A),S_REF(B))),S_IMPL(S_REF(A),S_REF(B)))),S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_REF(A),S_REF(B))))),
//((A->B)->((A->B)->(A->B))->(A->B))->((A->B)->(A->B))
        S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_REF(A),S_REF(B))),S_IMPL(S_REF(A),S_REF(B)))),S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_REF(A),S_REF(B)))),
//((A->B)->((A->B)->(A->B))->(A->B))
        S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_REF(A),S_REF(B))),S_IMPL(S_REF(A),S_REF(B)))),
//(A->B)->(A->B)
        S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_REF(A),S_REF(B))),
//(A->B)->B
        S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_REF(B)),
//!B->(A->B)->!B
        S_IMPL(S_NEG(S_REF(B)),S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_NEG(S_REF(B)))),
//(A->B)->!B
        S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_NEG(S_REF(B))),
//((A->B)->!B)->!(A->B)
        S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_NEG(S_REF(B))),S_NEG(S_IMPL(S_REF(A),S_REF(B)))),
//!(A->B)
        S_NEG(S_IMPL(S_REF(A),S_REF(B))),
};
sh_list NAB_IMPL {
//B->A->B
        S_IMPL(S_REF(B),S_IMPL(S_REF(A),S_REF(B))),
//A->B
        S_IMPL(S_REF(A),S_REF(B))
};
sh_list NANB_IMPL {
//(A->!B->A)->A->(A->!B->A)
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_REF(A))),S_IMPL(S_REF(A),S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_REF(A))))),
//(A->!B->A)
        S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_REF(A))),
//A->(A->!B->A)
        S_IMPL(S_REF(A),S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_REF(A)))),
//A->A->A
        S_IMPL(S_REF(A),S_IMPL(S_REF(A),S_REF(A))),
//(A->A->A)->(A->(A->A)->A)->(A->A)
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_REF(A),S_REF(A))),S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_REF(A),S_REF(A)),S_REF(A))),S_IMPL(S_REF(A),S_REF(A)))),
//(A->(A->A)->A)->(A->A)
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_REF(A),S_REF(A)),S_REF(A))),S_IMPL(S_REF(A),S_REF(A))),
//A->(A->A)->A
        S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_REF(A),S_REF(A)),S_REF(A))),
//A->A
        S_IMPL(S_REF(A),S_REF(A)),
//(A->A)->(A->A->(!B->A))->(A->(!B->A))
        S_IMPL(S_IMPL(S_REF(A),S_REF(A)),S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_REF(A)))),S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_REF(A))))),
//(A->A->!B->A)->(A->!B->A)
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_REF(A)))),S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_REF(A)))),
//A->!B->A
        S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_REF(A))),
//(!A->!B->!A)->A->(!A->!B->!A)
        S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A)))))),
//!A->!B->!A
        S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A)))),
//A->!A->!B->!A
        S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))))),
//!A->A->!A
        S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_REF(A),S_NEG(S_REF(A)))),
//A->!A
        S_IMPL(S_REF(A),S_NEG(S_REF(A))),
//(A->!A)->(A->!A->!B->!A)->(A->!B->!A)
        S_IMPL(S_IMPL(S_REF(A),S_NEG(S_REF(A))),S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))))),S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A)))))),
//(A->!A->!B->!A)->(A->!B->!A)
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))))),S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))))),
//A->!B->!A
        S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A)))),
//((!B->A)->(!B->!A)->!!B)->A->((!B->A)->(!B->!A)->!!B)
        S_IMPL(S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_REF(A)),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(B))))),S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_REF(A)),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(B))))))),
//(!B->A)->(!B->!A)->!!B
        S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_REF(A)),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(B))))),
//A->((!B->A)->(!B->!A)->!!B)
        S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_REF(A)),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(B)))))),
//(A->(!B->A))->(A->(!B->A)->((!B->!A)->!!B))->(A->((!B->!A)->!!B))
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_REF(A))),S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_REF(A)),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(B)))))),S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(B))))))),
//((A->((!B->A)->((!B->!A)->!!B)))->(A->((!B->!A)->!!B)))
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_REF(A)),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(B)))))),S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(B)))))),
//A->((!B->!A)->!!B)
        S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(B))))),
//(A->(!B->!A))->(A->(!B->!A)->!!B)->(A->!!B)
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(B))))),S_IMPL(S_REF(A),S_NEG(S_NEG(S_REF(B)))))),
//((A->((!B->!A)->!!B))->(A->!!B))
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(B))))),S_IMPL(S_REF(A),S_NEG(S_NEG(S_REF(B))))),
//A->!!B
        S_IMPL(S_REF(A),S_NEG(S_NEG(S_REF(B)))),
//(!!B->B)->A->(!!B->B)
        S_IMPL(S_IMPL(S_NEG(S_NEG(S_REF(B))),S_REF(B)),S_IMPL(S_REF(A),S_IMPL(S_NEG(S_NEG(S_REF(B))),S_REF(B)))),
//!!B->B
        S_IMPL(S_NEG(S_NEG(S_REF(B))),S_REF(B)),
//A->!!B->B
        S_IMPL(S_REF(A),S_IMPL(S_NEG(S_NEG(S_REF(B))),S_REF(B))),
//(A->!!B)->(A->!!B->B)->(A->B)
        S_IMPL(S_IMPL(S_REF(A),S_NEG(S_NEG(S_REF(B)))),S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_NEG(S_NEG(S_REF(B))),S_REF(B))),S_IMPL(S_REF(A),S_REF(B)))),
//((A->!!B->B)->(A->B))
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_NEG(S_NEG(S_REF(B))),S_REF(B))),S_IMPL(S_REF(A),S_REF(B))),
//A->B
        S_IMPL(S_REF(A),S_REF(B)),
};

/*
 *  conjunction
 */

sh_list AB_AND {
//A->B->A&B
        S_IMPL(S_REF(A),S_IMPL(S_REF(B),S_CONJ(S_REF(A),S_REF(B)))),
//B->A&B
        S_IMPL(S_REF(B),S_CONJ(S_REF(A),S_REF(B))),
//A&B
        S_CONJ(S_REF(A),S_REF(B)),
};
sh_list ANB_NAND {
//((A&B)->B)->((A&B)->!B)->!(A&B)
        S_IMPL(S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_REF(B)),S_IMPL(S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(B))),S_NEG(S_CONJ(S_REF(A),S_REF(B))))),
//(A&B)->B
        S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_REF(B)),
//(A&B->!B)->!(A&B)
        S_IMPL(S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(B))),S_NEG(S_CONJ(S_REF(A),S_REF(B)))),
//!B->A&B->!B
        S_IMPL(S_NEG(S_REF(B)),S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(B)))),
//A&B->!B
        S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(B))),
//!(A&B)
        S_NEG(S_CONJ(S_REF(A),S_REF(B)))
};
sh_list NAB_NAND {
//((A&B)->A)->((A&B)->!A)->!(A&B)
        S_IMPL(S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_REF(A)),S_IMPL(S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_CONJ(S_REF(A),S_REF(B))))),
//(A&B)->A
        S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_REF(A)),
//(A&B->!A)->!(A&B)
        S_IMPL(S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_CONJ(S_REF(A),S_REF(B)))),
//!A->A&B->!A
        S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A)))),
//A&B->!A
        S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A))),
//!(A&B)
        S_NEG(S_CONJ(S_REF(A),S_REF(B))),
};
sh_list NANB_NAND {
//((A&B)->A)->((A&B)->!A)->!(A&B)
        S_IMPL(S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_REF(A)),S_IMPL(S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_CONJ(S_REF(A),S_REF(B))))),
//(A&B)->A
        S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_REF(A)),
//(A&B->!A)->!(A&B)
        S_IMPL(S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_CONJ(S_REF(A),S_REF(B)))),
//!A->A&B->!A
        S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A)))),
//A&B->!A
        S_IMPL(S_CONJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A))),
//!(A&B)
        S_NEG(S_CONJ(S_REF(A),S_REF(B))),
};

/*
 *  negation
 */

sh_list A_NNOT {
//!A->(!A->!A)->!A
        S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(A))),S_NEG(S_REF(A)))),
//!A->!A->!A
        S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(A)))),
//(!A->!A->!A)->(!A->(!A->!A)->!A)->(!A->!A)
        S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(A))),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(A))))),
//(!A->(!A->!A)->!A)->(!A->!A)
        S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(A))),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(A)))),
//!A->!A
        S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(A))),
//A->!A->A
        S_IMPL(S_REF(A),S_IMPL(S_NEG(S_REF(A)),S_REF(A))),
//!A->A
        S_IMPL(S_NEG(S_REF(A)),S_REF(A)),
//(!A->A)->(!A->!A)->!!A
        S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_REF(A)),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(A))))),
//(!A->!A)->!!A
        S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(A))),S_NEG(S_NEG(S_REF(A)))),
//!!A
        S_NEG(S_NEG(S_REF(A))),
};
sh_list NA_NOT {
//!A
        S_NEG(S_REF(A))
};

/*
 *  disjunction
 */
sh_list AB_OR {
//A->A|B
        S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_REF(B))),
//A|B
        S_DISJ(S_REF(A),S_REF(B)),
};
sh_list ANB_OR {
//A->A|B
        S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_REF(B))),
//A|B
        S_DISJ(S_REF(A),S_REF(B)),
};
sh_list NAB_OR {
//B->A|B
        S_IMPL(S_REF(B),S_DISJ(S_REF(A),S_REF(B))),
//A|B
        S_DISJ(S_REF(A),S_REF(B)),
};
sh_list NANB_NOR {
//(A|B->(A))->(A|B->!(A))->!(A|B)
        S_IMPL(S_IMPL(S_DISJ(S_REF(A),S_REF(B)),S_REF(A)),S_IMPL(S_IMPL(S_DISJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_DISJ(S_REF(A),S_REF(B))))),
//(A->A)->(B->A)->(A|B->A)
        S_IMPL(S_IMPL(S_REF(A),S_REF(A)),S_IMPL(S_IMPL(S_REF(B),S_REF(A)),S_IMPL(S_DISJ(S_REF(A),S_REF(B)),S_REF(A)))),
//A->A->A
        S_IMPL(S_REF(A),S_IMPL(S_REF(A),S_REF(A))),
//(A->A->A)->(A->(A->A)->A)->(A->A)
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_REF(A),S_REF(A))),S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_REF(A),S_REF(A)),S_REF(A))),S_IMPL(S_REF(A),S_REF(A)))),
//(A->(A->A)->A)->(A->A)
        S_IMPL(S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_REF(A),S_REF(A)),S_REF(A))),S_IMPL(S_REF(A),S_REF(A))),
//A->(A->A)->A
        S_IMPL(S_REF(A),S_IMPL(S_IMPL(S_REF(A),S_REF(A)),S_REF(A))),
//A->A
        S_IMPL(S_REF(A),S_REF(A)),
//(B->A)->(A|B->A)
        S_IMPL(S_IMPL(S_REF(B),S_REF(A)),S_IMPL(S_DISJ(S_REF(A),S_REF(B)),S_REF(A))),
//(B->!A->B)->B->(B->!A->B)
        S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_REF(B))),S_IMPL(S_REF(B),S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_REF(B))))),
//(B->!A->B)
        S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_REF(B))),
//B->(B->!A->B)
        S_IMPL(S_REF(B),S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_REF(B)))),
//B->B->B
        S_IMPL(S_REF(B),S_IMPL(S_REF(B),S_REF(B))),
//(B->B->B)->(B->(B->B)->B)->(B->B)
        S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_REF(B),S_REF(B))),S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_REF(B),S_REF(B)),S_REF(B))),S_IMPL(S_REF(B),S_REF(B)))),
//(B->(B->B)->B)->(B->B)
        S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_REF(B),S_REF(B)),S_REF(B))),S_IMPL(S_REF(B),S_REF(B))),
//B->(B->B)->B
        S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_REF(B),S_REF(B)),S_REF(B))),
//B->B
        S_IMPL(S_REF(B),S_REF(B)),
//(B->B)->(B->B->(!A->B))->(B->(!A->B))
        S_IMPL(S_IMPL(S_REF(B),S_REF(B)),S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_REF(B)))),S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_REF(B))))),
//(B->B->!A->B)->(B->!A->B)
        S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_REF(B)))),S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_REF(B)))),
//B->!A->B
        S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_REF(B))),
//(!B->!A->!B)->B->(!B->!A->!B)
        S_IMPL(S_IMPL(S_NEG(S_REF(B)),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B)))),S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(B)),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B)))))),
//!B->!A->!B
        S_IMPL(S_NEG(S_REF(B)),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B)))),
//B->!B->!A->!B
        S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(B)),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))))),
//!B->B->!B
        S_IMPL(S_NEG(S_REF(B)),S_IMPL(S_REF(B),S_NEG(S_REF(B)))),
//B->!B
        S_IMPL(S_REF(B),S_NEG(S_REF(B))),
//(B->!B)->(B->!B->!A->!B)->(B->!A->!B)
        S_IMPL(S_IMPL(S_REF(B),S_NEG(S_REF(B))),S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(B)),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))))),S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B)))))),
//(B->!B->!A->!B)->(B->!A->!B)
        S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(B)),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))))),S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))))),
//B->!A->!B
        S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B)))),
//((!A->B)->(!A->!B)->!!A)->B->((!A->B)->(!A->!B)->!!A)
        S_IMPL(S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_REF(B)),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))),S_NEG(S_NEG(S_REF(A))))),S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_REF(B)),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))),S_NEG(S_NEG(S_REF(A))))))),
//(!A->B)->(!A->!B)->!!A
        S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_REF(B)),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))),S_NEG(S_NEG(S_REF(A))))),
//B->((!A->B)->(!A->!B)->!!A)
        S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_REF(B)),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))),S_NEG(S_NEG(S_REF(A)))))),
//(B->(!A->B))->(B->(!A->B)->((!A->!B)->!!A))->(B->((!A->!B)->!!A))
        S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_REF(B))),S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_REF(B)),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))),S_NEG(S_NEG(S_REF(A)))))),S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))),S_NEG(S_NEG(S_REF(A))))))),
//((B->((!A->B)->((!A->!B)->!!A)))->(B->((!A->!B)->!!A)))
        S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_REF(B)),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))),S_NEG(S_NEG(S_REF(A)))))),S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))),S_NEG(S_NEG(S_REF(A)))))),
//B->((!A->!B)->!!A)
        S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))),S_NEG(S_NEG(S_REF(A))))),
//(B->(!A->!B))->(B->(!A->!B)->!!A)->(B->!!A)
        S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B)))),S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))),S_NEG(S_NEG(S_REF(A))))),S_IMPL(S_REF(B),S_NEG(S_NEG(S_REF(A)))))),
//((B->((!A->!B)->!!A))->(B->!!A))
        S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_REF(B))),S_NEG(S_NEG(S_REF(A))))),S_IMPL(S_REF(B),S_NEG(S_NEG(S_REF(A))))),
//B->!!A
        S_IMPL(S_REF(B),S_NEG(S_NEG(S_REF(A)))),
//(!!A->A)->B->(!!A->A)
        S_IMPL(S_IMPL(S_NEG(S_NEG(S_REF(A))),S_REF(A)),S_IMPL(S_REF(B),S_IMPL(S_NEG(S_NEG(S_REF(A))),S_REF(A)))),
//!!A->A
        S_IMPL(S_NEG(S_NEG(S_REF(A))),S_REF(A)),
//B->!!A->A
        S_IMPL(S_REF(B),S_IMPL(S_NEG(S_NEG(S_REF(A))),S_REF(A))),
//(B->!!A)->(B->!!A->A)->(B->A)
        S_IMPL(S_IMPL(S_REF(B),S_NEG(S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_NEG(S_NEG(S_REF(A))),S_REF(A))),S_IMPL(S_REF(B),S_REF(A)))),
//((B->!!A->A)->(B->A))
        S_IMPL(S_IMPL(S_REF(B),S_IMPL(S_NEG(S_NEG(S_REF(A))),S_REF(A))),S_IMPL(S_REF(B),S_REF(A))),
//B->A
        S_IMPL(S_REF(B),S_REF(A)),
//A|B->A
        S_IMPL(S_DISJ(S_REF(A),S_REF(B)),S_REF(A)),
//(A|B->!A)->!(A|B)
        S_IMPL(S_IMPL(S_DISJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A))),S_NEG(S_DISJ(S_REF(A),S_REF(B)))),
//!A->A|B->!A
        S_IMPL(S_NEG(S_REF(A)),S_IMPL(S_DISJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A)))),
//A|B->!A
        S_IMPL(S_DISJ(S_REF(A),S_REF(B)),S_NEG(S_REF(A))),
//!(A|B)
        S_NEG(S_DISJ(S_REF(A),S_REF(B))),
};

sh_list SUP_REDUCE {
//A->A|!A
        S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),
//(((A->(A|!A))->((A->!(A|!A))->!A))->(!(A|!A)->((A->(A|!A))->((A->!(A|!A))->!A))))
        S_IMPL(S_IMPL(S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_REF(A)))))),
//((A->(A|!A))->((A->!(A|!A))->!A))
        S_IMPL(S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_REF(A)))),
//(!(A|!A)->((A->(A|!A))->((A->!(A|!A))->!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_REF(A))))),
//((A->(A|!A))->(!(A|!A)->(A->(A|!A))))
        S_IMPL(S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//(A->(A|!A))
        S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),
//(!(A|!A)->(A->(A|!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A))))),
//((!(A|!A)->(A->(A|!A)))->((!(A|!A)->((A->(A|!A))->((A->!(A|!A))->!A)))->(!(A|!A)->((A->!(A|!A))->!A))))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_REF(A))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_REF(A)))))),
//((!(A|!A)->((A->(A|!A))->((A->!(A|!A))->!A)))->(!(A|!A)->((A->!(A|!A))->!A)))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_REF(A))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_REF(A))))),
//(!(A|!A)->((A->!(A|!A))->!A))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_REF(A)))),
//((!(A|!A)->(A->!(A|!A)))->(!(A|!A)->(!(A|!A)->(A->!(A|!A)))))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))))),
//(!(A|!A)->(A->!(A|!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//(!(A|!A)->(!(A|!A)->(A->!(A|!A))))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))))),
//(!(A|!A)->(!(A|!A)->!(A|!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//((!(A|!A)->(!(A|!A)->!(A|!A)))->((!(A|!A)->((!(A|!A)->!(A|!A))->!(A|!A)))->(!(A|!A)->!(A|!A))))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))))),
//((!(A|!A)->((!(A|!A)->!(A|!A))->!(A|!A)))->(!(A|!A)->!(A|!A)))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//(!(A|!A)->((!(A|!A)->!(A|!A))->!(A|!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//(!(A|!A)->!(A|!A))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),
//((!(A|!A)->!(A|!A))->((!(A|!A)->(!(A|!A)->(A->!(A|!A))))->(!(A|!A)->(A->!(A|!A)))))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))))),
//((!(A|!A)->(!(A|!A)->(A->!(A|!A))))->(!(A|!A)->(A->!(A|!A))))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))))),
//(!(A|!A)->(A->!(A|!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//((!(A|!A)->(A->!(A|!A)))->((!(A|!A)->((A->!(A|!A))->!A))->(!(A|!A)->!A)))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_REF(A))))),
//((!(A|!A)->((A->!(A|!A))->!A))->(!(A|!A)->!A))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_REF(A),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_REF(A)))),
//(!(A|!A)->!A)
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_REF(A))),
//!A->A|!A
        S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),
//(((!A->(A|!A))->((!A->!(A|!A))->!!A))->(!(A|!A)->((!A->(A|!A))->((!A->!(A|!A))->!!A))))
        S_IMPL(S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_NEG(S_REF(A))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_NEG(S_REF(A))))))),
//((!A->(A|!A))->((!A->!(A|!A))->!!A))
        S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_NEG(S_REF(A))))),
//(!(A|!A)->((!A->(A|!A))->((!A->!(A|!A))->!!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_NEG(S_REF(A)))))),
//((!A->(A|!A))->(!(A|!A)->(!A->(A|!A))))
        S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//(!A->(A|!A))
        S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),
//(!(A|!A)->(!A->(A|!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A))))),
//((!(A|!A)->(!A->(A|!A)))->((!(A|!A)->((!A->(A|!A))->((!A->!(A|!A))->!!A)))->(!(A|!A)->((!A->!(A|!A))->!!A))))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_NEG(S_REF(A)))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_NEG(S_REF(A))))))),
//((!(A|!A)->((!A->(A|!A))->((!A->!(A|!A))->!!A)))->(!(A|!A)->((!A->!(A|!A))->!!A)))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_NEG(S_REF(A)))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_NEG(S_REF(A)))))),
//(!(A|!A)->((!A->!(A|!A))->!!A))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_NEG(S_REF(A))))),
//((!(A|!A)->(!A->!(A|!A)))->(!(A|!A)->(!(A|!A)->(!A->!(A|!A)))))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))))),
//(!(A|!A)->(!A->!(A|!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//(!(A|!A)->(!(A|!A)->(!A->!(A|!A))))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))))),
//(!(A|!A)->(!(A|!A)->!(A|!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//((!(A|!A)->(!(A|!A)->!(A|!A)))->((!(A|!A)->((!(A|!A)->!(A|!A))->!(A|!A)))->(!(A|!A)->!(A|!A))))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))))),
//((!(A|!A)->((!(A|!A)->!(A|!A))->!(A|!A)))->(!(A|!A)->!(A|!A)))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//(!(A|!A)->((!(A|!A)->!(A|!A))->!(A|!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//(!(A|!A)->!(A|!A))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),
//((!(A|!A)->!(A|!A))->((!(A|!A)->(!(A|!A)->(!A->!(A|!A))))->(!(A|!A)->(!A->!(A|!A)))))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))))),
//((!(A|!A)->(!(A|!A)->(!A->!(A|!A))))->(!(A|!A)->(!A->!(A|!A))))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))))),
//(!(A|!A)->(!A->!(A|!A)))
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//((!(A|!A)->(!A->!(A|!A)))->((!(A|!A)->((!A->!(A|!A))->!!A))->(!(A|!A)->!!A)))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_NEG(S_REF(A))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_NEG(S_REF(A)))))),
//((!(A|!A)->((!A->!(A|!A))->!!A))->(!(A|!A)->!!A))
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_NEG(S_NEG(S_REF(A))))),S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_NEG(S_REF(A))))),
//(!(A|!A)->!!A)
        S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_NEG(S_REF(A)))),
//(!(A|!A)->!A)->(!(A|!A)->!!A)->!!(A|!A)
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_REF(A))),S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_NEG(S_REF(A)))),S_NEG(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))))),
//(!(A|!A)->!!A)->!!(A|!A)
        S_IMPL(S_IMPL(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))),S_NEG(S_NEG(S_REF(A)))),S_NEG(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A)))))),
//!!(A|!A)
        S_NEG(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),
//!!(A|!A)->(A|!A)
        S_IMPL(S_NEG(S_NEG(S_DISJ(S_REF(A),S_NEG(S_REF(A))))),S_DISJ(S_REF(A),S_NEG(S_REF(A)))),
//A|!A
        S_DISJ(S_REF(A),S_NEG(S_REF(A))),
//(A->B)->(!A->B)->(A|!A)->B
        S_IMPL(S_IMPL(S_REF(A),S_REF(B)),S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_REF(B)),S_IMPL(S_DISJ(S_REF(A),S_NEG(S_REF(A))),S_REF(B)))),
//(!A->B)->(A|!A)->B
        S_IMPL(S_IMPL(S_NEG(S_REF(A)),S_REF(B)),S_IMPL(S_DISJ(S_REF(A),S_NEG(S_REF(A))),S_REF(B))),
//(A|!A)->B
        S_IMPL(S_DISJ(S_REF(A),S_NEG(S_REF(A))),S_REF(B)),
//B
        S_REF(B)
};

int is_axiom(expression const& expr) {
    for (int i = 0; i < 10; ++i) {
        if (AXIOM[i].match(expr))
            return i;
    }
    return -1;
}

std::vector<expression> get_sub_list(std::vector<expression_scheme> shemes, std::map<std::string, expression> const &m) {
    std::vector<expression> ret{};
    for (size_t i = 0; i < shemes.size(); ++i) {
        ret.push_back(shemes[i].get_expression(m));
    }
    return ret;
}

sh_list const &get_not_vsproof(bool arg0) {
    return arg0 ? A_NNOT : NA_NOT;
}

sh_list const &get_impl_vsproof(bool arg0, bool arg1) {
    return arg0 ? (arg1 ? AB_IMPL : ANB_NIMPL) : (arg1 ? NAB_IMPL : NANB_IMPL);
}

sh_list const &get_conj_vsproof(bool arg0, bool arg1) {
    return arg0 ? (arg1 ? AB_AND : ANB_NAND) : (arg1 ? NAB_NAND : NANB_NAND);
}

sh_list const &get_disj_vsproof(bool arg0, bool arg1) {
    return arg0 ? (arg1 ? AB_OR : ANB_OR) : (arg1 ? NAB_OR : NANB_NOR);
}

sh_list const &mp_deduction() {
    return MP_DEDUC;
}

sh_list const &aa_deduction() {
    return AA_DEDUC;
}

sh_list const &ax_deduction() {
    return AX_DEDUC;
}

sh_list const &suppose_reduce_proof() {
    return SUP_REDUCE;
}