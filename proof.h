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
        size_t operator()(expression const &e) const noexcept { return e->hash(); }
    };
}

struct ann_t {

    static const size_t ANN_UNDEF  = 0;
    static const size_t ANN_AXIOM  = 1;
    static const size_t ANN_SUPPOSE  = 2;
    static const size_t ANN_MODUS_PONENS  = 3;

    size_t type, arg0, arg1;

    ann_t(size_t type, size_t arg0 = 0, size_t arg1 = 0) : type(type), arg0(arg0), arg1(arg1) { }
    ann_t(ann_t const& other) : ann_t(other.type, other.arg0, other.arg1) { }
};

class proof {
    std::vector<expression> supposes, proof_list;
    expression statement;

    std::vector<ann_t> annotations;
    bool is_annotated, show_ann;

    friend proof* concat(proof*, proof*);
public:

    proof(const std::vector<expression> &supposes,
          const std::vector<expression> &proof_list,
          const expression &statement);

    proof(std::vector<expression> &&supposes,
          std::vector<expression> &&proof_list,
          expression const& statement);

    proof(proof const& other);

    explicit proof(std::istream& s);
    explicit proof(expression proovable);

    friend std::ostream & operator<<(std::ostream& ostream, proof const& _proof);

    inline proof& annotation(bool show_ann = true) {
        this->show_ann = show_ann;
        return *this;
    }

    void annotate();
    void deduce();
    void simplify();
};


#endif //MATHLOGIC_PROOF_H
