//
// Created by radimir on 08.01.16.
//

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "proof.h"
#include "axioms.h"
#include "parser.h"
#include "util.h"


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
    std::map<std::string, expression> aa_map{};
    aa_map["A"] = a;
    std::vector<expression> a_impl_a_proof = get_sub_list(aa_deduction(), aa_map);

    supposes.pop_back();

    std::vector<expression> n_proof_list{};
    std::vector<ann_t> n_annotations{};
    std::vector<size_t> links{};
    for (size_t i = 0; i < proof_list.size(); ++i) {
        if (annotations[i].type == ann_t::ANN_AXIOM || (annotations[i].type == ann_t::ANN_SUPPOSE && annotations[i].arg0 < supposes.size())) {
            ax_map["X"] = proof_list[i];
            push(n_proof_list, get_sub_list(ax_deduction(), ax_map));

//            n_annotations.push_back(annotations[i]);
//            n_annotations.push_back(ann_t(ann_t::ANN_AXIOM, 0));
//            n_annotations.push_back(ann_t(ann_t::ANN_MODUS_PONENS, n_annotations.size() - 2, n_annotations.size() - 1));
        } else if (annotations[i].type == ann_t::ANN_SUPPOSE && annotations[i].arg0 == supposes.size()){
            push(n_proof_list, a_impl_a_proof);

//            n_annotations.push_back(ann_t(ann_t::ANN_AXIOM, 0));
//            n_annotations.push_back(ann_t(ann_t::ANN_AXIOM, 1));
//            n_annotations.push_back(ann_t(ann_t::ANN_MODUS_PONENS, n_annotations.size() - 2, n_annotations.size() - 1));
//            n_annotations.push_back(ann_t(ann_t::ANN_AXIOM, 1));
//            n_annotations.push_back(ann_t(ann_t::ANN_MODUS_PONENS, n_annotations.size() - 1, n_annotations.size() - 2));
        } else if (annotations[i].type == ann_t::ANN_MODUS_PONENS) {
            mp_map["Y"] = proof_list[annotations[i].arg0];
            mp_map["Z"] = proof_list[i];
            push(n_proof_list, get_sub_list(mp_deduction(), mp_map));

//            n_annotations.push_back(ann_t(ann_t::ANN_AXIOM, 1));
//            n_annotations.push_back(ann_t(ann_t::ANN_MODUS_PONENS, links[annotations[i].arg0], n_annotations.size() - 1));
//            n_annotations.push_back(ann_t(ann_t::ANN_MODUS_PONENS, links[annotations[i].arg1], n_annotations.size() - 1));
        }
        links.push_back(n_annotations.size() - 1);
    }
    proof_list = std::move(n_proof_list);
    annotations = std::move(n_annotations);
    statement = E_IMPL(a, statement);
    is_annotated = false;
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
    if (p.lexem == parser<expression>::LEXEM_END) { throw std::exception{}; }
    if (p.lexem != parser<expression>::LEXEM_SEPARATOR) {
        supposes.push_back(p.parse_implication());
        while (p.lexem != parser<expression>::LEXEM_SEPARATOR) {
            p.nextLex();
            if (p.lexem == parser<expression>::LEXEM_END) { throw std::exception{}; }
            supposes.push_back(p.parse_implication());
        }
    }
    p.nextLex();
    statement = p.parse_implication();
    while (p.lexem == parser<expression>::LEXEM_NEW_LINE) {
        p.nextLex();
        if (p.lexem == parser<expression>::LEXEM_END) { break; }
        proof_list.push_back(p.parse_implication());
    }
}


std::vector<expression> make_supposes(std::vector<variable *> const &vars) {
    std::vector<expression> ret{};
    for (auto it = vars.rbegin(); it != vars.rend(); ++it) {
        expression ref = make_variable_ref(*it);
        ret.push_back(((*it)->value) ? ref : E_NEG(ref));
    }
    return ret;
}

proof* concat(proof *p1, proof *p2) {
    assert(p1->statement == p2->statement);
    expression removed = make_variable_ref(p2->supposes.back().get_variables().front());
    expression statement = p2->statement;
    p1->deduce();
    p2->deduce();
    push(p1->proof_list, p2->proof_list);
    std::map<std::string, expression> m{std::make_pair("A", removed), std::make_pair("B", statement)};
    push(p1->proof_list,get_sub_list(suppose_reduce_proof(), m));
    delete p2;
    p1->is_annotated = false;
    p1->statement = statement;
    return p1;
};

proof::proof(expression proovable)
    : supposes{},
      statement{proovable},
      proof_list{},
      is_annotated(false),
      show_ann(false)
{
    std::vector<variable *> vars = proovable.get_variables();
    size_t count = vars.size();
    bit_tuple values{count};

    std::vector<proof*> proofs{};

    values.do_while(
            [&proofs, &vars, this](bool const *vals, size_t c) -> bool {
                for (size_t k = 0; k < c; ++k) {
                    vars[k]->value = vals[k];
                }
                std::vector<expression> vsproof_list = statement->build_vsproof().first;
                std::vector<expression> vssupposes = make_supposes(vars);
                proofs.push_back(new proof(std::vector<expression>{vssupposes}, std::move(vsproof_list), statement));
                return true;
            }
    );
    while (proofs.size() > 1) {
        std::vector<proof*> n_proofs{};
        for (size_t i = 0; i < proofs.size(); i += 2) {
            n_proofs.push_back(concat(proofs[i], proofs[i + 1]));
        }
        proofs = std::move(n_proofs);
    }
    proof_list = std::move(proofs[0]->proof_list);
    delete proofs[0];
}

void proof::simplify() {
    if (!is_annotated) {
        annotate();
    }
    std::queue<size_t > q;
    bool flags[annotations.size()];
    for (size_t i = 0; i < annotations.size(); ++i) { flags[i] = false; }
    flags[annotations.size() - 1] = true;
    q.push(annotations.size() - 1);
    while (!q.empty()) {
        flags[q.front()] = true;
        ann_t ann = annotations[q.front()];
        q.pop();
        if (ann.type == ann_t::ANN_MODUS_PONENS) {
            q.push(ann.arg0);
            q.push(ann.arg1);
        }
    }
    std::vector<expression> n_proof_list{};
    for (size_t i = 0; i < annotations.size(); ++i) {
        if (flags[i]) {
            n_proof_list.push_back(proof_list[i]);
        }
    }
    is_annotated = false;
    proof_list = std::move(n_proof_list);
}