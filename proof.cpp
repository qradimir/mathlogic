//
// Created by radimir on 08.01.16.
//

#include "proof.h"
#include "axioms.h"

#define ANN_UNDEF 0
#define ANN_AXIOM 1
#define ANN_SUPPOSE 2
#define ANN_MODUS_PONENS 3

std::ostream& operator<<(std::ostream& stream, _ann const& ann) {
    switch (ann.type) {
        case ANN_UNDEF :
            return  stream << "Не доказано";
        case ANN_AXIOM :
            return stream << "Сх. акс. " << std::to_string(ann.arg0 + 1);
        case ANN_SUPPOSE :
            return stream << "Предп. " << std::to_string(ann.arg0 + 1);
        case ANN_MODUS_PONENS :
            return stream << "M.P. " <<std::to_string(ann.arg0 + 1) << ", " << std::to_string(ann.arg1 + 1);
        default:
            return stream;
    }
}

proof::proof(const std::vector<expression> &supposes,
             const std::vector<expression> &proof_list,
             const expression &statement) :
        supposes(supposes),
        proof_list(proof_list),
        statement(statement),
        is_annotated(false),
        _annotation(false)
{ }

proof::proof(proof const &other)
:   proof(other.supposes, other.proof_list, other.statement)
{
}

std::ostream &operator<<(std::ostream& ostream, proof const&_) {
    if (_.supposes.size() > 0) {
        ostream << _.supposes[0];
        for (size_t i = 1; i != _.supposes.size(); ++i) {
            ostream << "," << _.supposes[i];
        }
    }
    ostream << "|-" << _.statement << '\n';
    for(size_t i  = 0; i < _.proof_list.size(); ++i) {
        if (_._annotation && _.is_annotated) {
            ostream << _.ann_proof_list[i] << '\n';
        } else {
            ostream << _.proof_list[i] << '\n';
        }
    }
    return ostream;
}

annotated_expression::annotated_expression(const expression &e, size_t num, const _ann &ann)
        : e(e), num(num), ann(ann) { }

std::ostream &operator<<(std::ostream &ostream, annotated_expression const &ann_expr) {
    return ostream << "(" << ann_expr.num << ") " << ann_expr.e << " (" << ann_expr.ann << ")";
}

void proof::annotate() {
    if (is_annotated)
        return;

    int ax_num;
    for (size_t i = 0; i < proof_list.size(); ++i) {
        if ((ax_num = is_axiom(proof_list[i])) >= 0) {
            ann_proof_list.push_back(annotated_expression(proof_list[i], i + 1, _ann(ANN_AXIOM, (size_t)ax_num)));
            continue;
        }
        bool found = false;
        for (size_t j = 0; j < supposes.size(); ++j) {
            if (proof_list[i] == supposes[j]) {
                ann_proof_list.push_back(annotated_expression(proof_list[i], i + 1, _ann(ANN_SUPPOSE, j)));
                found = true;
                break;
            }
        }
        if (found) continue;
        for (size_t j = 0; j < i; ++j) {
            if (ann_proof_list[j].ann.type == ANN_UNDEF) {
                continue;
            }
            expression t = make_operation(get_implication(), proof_list[j], proof_list[i]);
            for (size_t k = 0; k < i; ++k) {
                if (t == proof_list[k]) {
                    ann_proof_list.push_back(annotated_expression(proof_list[i], i + 1, _ann(ANN_MODUS_PONENS, j, k)));
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        if (found) continue;
        ann_proof_list.push_back(annotated_expression(proof_list[i], i + 1, _ann(ANN_UNDEF)));
    }

    is_annotated = true;
}