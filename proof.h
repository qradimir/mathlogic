//
// Created by radimir on 08.01.16.
//

#ifndef MATHLOGIC_PROOF_H
#define MATHLOGIC_PROOF_H

#include <vector>
#include <ostream>
#include "expression.h"

class proof {

    std::vector<expression> supposes, proof_list;
    expression statement;

public:

    proof(const std::vector<expression> &supposes,
          const std::vector<expression> &proof_list,
          const expression &statement);

    proof(proof const& other);

    friend std::ostream & operator<<(std::ostream& ostream, proof const& _proof);
};


#endif //MATHLOGIC_PROOF_H
