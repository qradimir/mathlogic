//
// Created by radimir on 08.01.16.
//

#ifndef MATHLOGIC_AXIOMAS_H
#define MATHLOGIC_AXIOMAS_H

#include "expression.h"
#include "expression_scheme.h"
#include <vector>

#define SHLIST(name) static const expression_scheme name[]

SHLIST(AXIOM){
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
SHLIST(AA_IMPL){
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
SHLIST(AXIOM_UTIL){
//  Y->X
        S_IMPL(S_REF(Y), S_REF(X)),
//  (X->Y->Z)->(X->Z)
        S_IMPL(S_IMPL(S_REF(X), S_IMPL(S_REF(Y), S_REF(Z))), S_IMPL(S_REF(X), S_REF(Z))),
//  X->Z
        S_IMPL(S_REF(X), S_REF(Z))
};

/*
 *  implication
 */

SHLIST(AB_IMPL){
//B->A->B
        S_IMPL(S_REF(B),S_IMPL(S_REF(A),S_REF(B))),
//A->B
        S_IMPL(S_REF(A),S_REF(B))
};
SHLIST(ANB_NIMPL){
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
SHLIST(NAB_IMPL) {
//B->A->B
            S_IMPL(S_REF(B),S_IMPL(S_REF(A),S_REF(B))),
//A->B
            S_IMPL(S_REF(A),S_REF(B))
};
SHLIST(NANB_IMPL) {
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

SHLIST(AB_AND) {
//A->B->A&B
        S_IMPL(S_REF(A),S_IMPL(S_REF(B),S_CONJ(S_REF(A),S_REF(B)))),
//B->A&B
        S_IMPL(S_REF(B),S_CONJ(S_REF(A),S_REF(B))),
//A&B
        S_CONJ(S_REF(A),S_REF(B)),
};
SHLIST(ANB_NAND) {
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
SHLIST(NAB_NAND) {
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
SHLIST(NANB_NAND) {
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

SHLIST(A_NNOT) {
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
SHLIST(NA_NOT) {};

/*
 *  disjunction
 */
SHLIST(AB_OR) {
//A->A|B
        S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_REF(B))),
//A|B
        S_DISJ(S_REF(A),S_REF(B)),
};
SHLIST(ANB_OR) {
//A->A|B
        S_IMPL(S_REF(A),S_DISJ(S_REF(A),S_REF(B))),
//A|B
        S_DISJ(S_REF(A),S_REF(B)),
};
SHLIST(NAB_OR) {
//B->A|B
        S_IMPL(S_REF(B),S_DISJ(S_REF(A),S_REF(B))),
//A|B
        S_DISJ(S_REF(A),S_REF(B)),
};
SHLIST(NANB_NOR) {
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

std::vector<expression> get_a_impl_a_proof(expression const& sub);
int is_axiom(expression const& expr);


#endif //MATHLOGIC_AXIOMAS_H
