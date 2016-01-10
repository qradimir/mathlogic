//
// Created by radimir on 08.01.16.
//

#ifndef MATHLOGIC_PROOF_H
#define MATHLOGIC_PROOF_H

#include <vector>
#include <ostream>
#include "expression.h"

namespace std {
    template<>
    struct hash<expression> {
        size_t operator()(expression e) const noexcept { return e->hash(); }
    };
}

struct _ann {
    size_t type, arg0, arg1;

    _ann(size_t type, size_t arg0 = 0, size_t arg1 = 0) : type(type), arg0(arg0), arg1(arg1) { }
    _ann(_ann const& other) : _ann(other.type, other.arg0, other.arg1) { }
};

class proof {

    std::vector<expression> supposes, proof_list;
    expression statement;

    std::vector<_ann> annotations;
    bool is_annotated, _annotation;

public:

    proof(const std::vector<expression> &supposes,
          const std::vector<expression> &proof_list,
          const expression &statement);

    proof(std::vector<expression> &&supposes, std::vector<expression> &&proof_list, expression const& statement);

    proof(proof const& other);

    friend std::ostream & operator<<(std::ostream& ostream, proof const& _proof);

    inline proof& annotation(bool _annotation = true) {
        this->_annotation = _annotation;
        return *this;
    }

    void annotate();
};


#endif //MATHLOGIC_PROOF_H
