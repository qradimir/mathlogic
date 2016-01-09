//
// Created by radimir on 08.01.16.
//

#ifndef MATHLOGIC_PROOF_H
#define MATHLOGIC_PROOF_H

#include <vector>
#include <ostream>
#include "expression.h"

struct _ann {
    size_t type, arg0, arg1;

    _ann(size_t type, size_t arg0 = 0, size_t arg1 = 0) : type(type), arg0(arg0), arg1(arg1) { }
    _ann(_ann const& other) : _ann(other.type, other.arg0, other.arg1) { }
};

class annotated_expression {
public:
    const expression e;
    const size_t num;
    const _ann ann;

    annotated_expression(const expression &e, size_t num, const _ann &ann);

    friend std::ostream& operator<<(std::ostream& ostream, annotated_expression const& ann_expr);
};

class proof {

    std::vector<expression> supposes, proof_list;
    expression statement;

    std::vector<annotated_expression> ann_proof_list;
    bool is_annotated, _annotation;

public:

    proof(const std::vector<expression> &supposes,
          const std::vector<expression> &proof_list,
          const expression &statement);

    proof(proof const& other);

    friend std::ostream & operator<<(std::ostream& ostream, proof const& _proof);

    inline proof& annotation(bool _annotation = true) {
        this->_annotation = _annotation;
        return *this;
    }

    void annotate();
};


#endif //MATHLOGIC_PROOF_H
