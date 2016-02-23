//
// Created by radimir on 08.01.16.
//

#include <iostream>
#include <unordered_set>
#include <bits/unordered_map.h>
#include "proof.h"
#include "axioms.h"



std::ostream& operator<<(std::ostream& stream, ann_t const& ann) {
    switch (ann.type) {
        case ann_t::ANN_UNDEF :
            return  stream << "Не доказано";
        case ann_t::ANN_AXIOM :
            return stream << "Сх. акс. " << std::to_string(ann.arg0 + 1);
        case ann_t::ANN_SUPPOSE :
            return stream << "Предп. " << std::to_string(ann.arg0 + 1);
        case ann_t::ANN_MODUS_PONENS :
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
        show_ann(false)
{ }


proof::proof(std::vector<expression> &&supposes,
             std::vector<expression> &&proof_list,
             const expression& statement) :
        supposes(std::move(supposes)),
        proof_list(std::move(proof_list)),
        statement(statement),
        is_annotated(false),
        show_ann(false)
{ }

proof::proof(proof const &other)
:   proof(other.supposes, other.proof_list, other.statement)
{
}

std::ostream &operator<<(std::ostream& ostream, proof const &pr) {
    if (pr.supposes.size() > 0) {
        ostream << pr.supposes[0];
        for (size_t i = 1; i != pr.supposes.size(); ++i) {
            ostream << "," << pr.supposes[i];
        }
    }
    ostream << "|-" << pr.statement << '\n';
    for(size_t i  = 0; i < pr.proof_list.size(); ++i) {
        if (pr.show_ann && pr.is_annotated) {
            ostream << "(" << i + 1 << ") " << pr.proof_list[i] << " (" << pr.annotations[i] << ")" << '\n';
        } else {
            ostream << pr.proof_list[i] << '\n';
        }
    }
    return ostream;
}

void proof::annotate() {
    if (is_annotated) {
        return;
    }
    int ax_num;
    std::unordered_map<expression, int> expr_set{proof_list.size(), std::hash<expression>{}};
    for (size_t i = 0; i < proof_list.size(); ++i) {
        bool found = false;
        if ((ax_num = is_axiom(proof_list[i])) >= 0) {
            annotations.push_back(ann_t(ann_t::ANN_AXIOM, (size_t)ax_num));
            found = true;
        }
        if (!found){
            for (size_t j = 0; j < supposes.size(); ++j) {
                if (proof_list[i] == supposes[j]) {
                    annotations.push_back(ann_t(ann_t::ANN_SUPPOSE, j));
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            for (auto it = expr_set.begin(); it != expr_set.end(); ++it) {
                auto _expr = expr_set.find(make_operation(get_implication(), it->first, proof_list[i]));
                if (_expr != expr_set.end()) {
                    annotations.push_back(ann_t(ann_t::ANN_MODUS_PONENS, it->second, _expr->second));
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            annotations.push_back(ann_t(ann_t::ANN_UNDEF));
        } else {
            expr_set.insert(std::make_pair(proof_list[i], i));
        }
    }

    is_annotated = true;
}

void proof::deduce() {
    if (supposes.empty()) {
        return;
    }
    annotate();

    expression a = supposes.back();

    std::map<std::string, expression> ax_map{};
    ax_map["Y"] = a;
    std::map<std::string, expression> mp_map{};
    mp_map["X"] = a;
    std::vector<expression> a_impl_a_proof = get_a_impl_a_proof(a);

    supposes.pop_back();

    std::vector<expression> n_proof_list{};
    std::vector<ann_t> n_annotations{};
    std::vector<size_t> links{};
    for (size_t i = 0; i < proof_list.size(); ++i) {
        if (annotations[i].type == ann_t::ANN_AXIOM || (annotations[i].type == ann_t::ANN_SUPPOSE && annotations[i].arg0 < supposes.size())) {
            ax_map["X"] = proof_list[i];

            n_proof_list.push_back(proof_list[i]);
            n_proof_list.push_back(get_axioms()[0].get_expression(ax_map));
            n_proof_list.push_back(mp_scheme_0.get_expression(ax_map));

            n_annotations.push_back(annotations[i]);
            n_annotations.push_back(ann_t(ann_t::ANN_AXIOM, 0));
            n_annotations.push_back(ann_t(ann_t::ANN_MODUS_PONENS, n_annotations.size() - 2, n_annotations.size() - 1));
        } else if (annotations[i].type == ann_t::ANN_SUPPOSE && annotations[i].arg0 == supposes.size()){
            n_proof_list.insert(n_proof_list.end(), a_impl_a_proof.begin(), a_impl_a_proof.end());

            n_annotations.push_back(ann_t(ann_t::ANN_AXIOM, 0));
            n_annotations.push_back(ann_t(ann_t::ANN_AXIOM, 1));
            n_annotations.push_back(ann_t(ann_t::ANN_MODUS_PONENS, n_annotations.size() - 2, n_annotations.size() - 1));
            n_annotations.push_back(ann_t(ann_t::ANN_AXIOM, 1));
            n_annotations.push_back(ann_t(ann_t::ANN_MODUS_PONENS, n_annotations.size() - 1, n_annotations.size() - 2));
        } else if (annotations[i].type == ann_t::ANN_MODUS_PONENS) {
            mp_map["Y"] = proof_list[annotations[i].arg0];
            mp_map["Z"] = proof_list[i];
            n_proof_list.push_back(get_axioms()[1].get_expression(mp_map));
            n_proof_list.push_back(mp_scheme_1.get_expression(mp_map));
            n_proof_list.push_back(mp_mp_scheme_1.get_expression(mp_map));

            n_annotations.push_back(ann_t(ann_t::ANN_AXIOM, 1));
            n_annotations.push_back(ann_t(ann_t::ANN_MODUS_PONENS, links[annotations[i].arg0], n_annotations.size() - 1));
            n_annotations.push_back(ann_t(ann_t::ANN_MODUS_PONENS, links[annotations[i].arg1], n_annotations.size() - 1));
        }
        links.push_back(n_annotations.size() - 1);
    }
    proof_list = std::move(n_proof_list);
    annotations = std::move(n_annotations);
    statement = make_operation(get_implication(), a, statement);
}

proof::proof(std::istream& s)
    : supposes{},
      statement{nullptr},
      proof_list{},
      is_annotated(false),
      show_ann(false)
{
    parser<expression> p = get_expression_parser();
    p.input = &s;
    p.nextLex();
    if (p.lexem != parser<expression>::LEXEM_SEPARATOR) {
        supposes.push_back(p.parse_implication());
        while (p.lexem != parser<expression>::LEXEM_SEPARATOR) {
            p.nextLex();
            supposes.push_back(p.parse_implication());
        }
    }
    p.nextLex();
    statement = p.parse_implication();
    while (p.lexem == parser<expression>::LEXEM_NEW_LINE) {
        p.nextLex();
        if (p.lexem == parser<expression>::LEXEM_END) {
            break;
        }
        proof_list.push_back(p.parse_implication());
    }
}
