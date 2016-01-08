//
// Created by radimir on 08.01.16.
//

#include "proof.h"

proof::proof(const std::vector<expression> &supposes,
             const std::vector<expression> &proof_list,
             const expression &statement) :
        supposes(supposes),
        proof_list(proof_list),
        statement(statement)
{ }

proof::proof(proof const &other)
:   proof(other.supposes, other.proof_list, other.statement)
{
}

std::ostream friend &operator<<(std::ostream& ostream, proof const&_) {
    if (_.supposes.size() > 0) {
        ostream << _.supposes[0];
        for (size_t i = 1; i != _.supposes.size(); ++i) {
            ostream << "," << _.supposes[i];
        }
    }
    ostream << "|-" << _.statement << '\n';
    for(auto it  = _.proof_list.begin(); it != _.proof_list.end(); ++it) {
        ostream << *it << '\n';
    }
    return ostream;
}