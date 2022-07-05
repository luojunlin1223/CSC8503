#pragma once
#include <string>
#include <vector>
struct easy_prop {
    enum t {
        E_P_AND,
        E_P_OR,
        E_P_ATOM,
        E_P_TRUE,
        E_P_FALSE
    };
    t casusu;
    bool isAtomNegated;
    std::vector<easy_prop> args;
    std::string single_atom_if_any;

    easy_prop(t casusu);
    easy_prop();
    easy_prop(const easy_prop&) = default;
    easy_prop(easy_prop&&) = default;
    easy_prop& operator=(const easy_prop&) = default;
    easy_prop& operator=(easy_prop&&) = default;
};

easy_prop::easy_prop() : casusu{ E_P_TRUE }, isAtomNegated{ false }, args{}, single_atom_if_any{} {}
easy_prop::easy_prop(t casusu) : casusu{ casusu } {}

/*
 * ltlf.h
 * This file is part of bpm21
 *
 * Copyright (C) 2021 - Giacomo Bergami
 *
 * bpm21 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * bpm21 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bpm21. If not, see <http://www.gnu.org/licenses/>.
 */


 //
 // Created by giacomo on 18/02/21.
 //



#include <unordered_set>

#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif

enum formula_t {
    ACT,
    NEG_OF,
    OR,
    AND,
    NEXT,
    UNTIL,
    RELEASE,
    TRUE,
    FALSE,
    NUMERIC_ATOM,
    BOX,
    DIAMOND,
    LAST
};



#include <string>
#include <vector>
#include <ostream>
#include <cassert>


struct ltlf {
    formula_t              casusu;
    std::string            act;
    std::vector<std::string> rewritten_act;
    std::vector<ltlf>      args;
    bool                   is_negated;

    // C++ constructors
    ltlf();
    ltlf(const std::string& act);
    ltlf(formula_t citki);
    ltlf(const ltlf&) = default;
    ltlf(ltlf&&) = default;
    ltlf& operator=(const ltlf&) = default;
    ltlf& operator=(ltlf&&) = default;


    // Semantic constructors
    static struct ltlf True();
    static struct ltlf Act(const std::string& act, bool isNegated = false);
    static struct ltlf Neg(const ltlf& sub);
    static struct ltlf Next(const ltlf& sub);
    static struct ltlf Or(const ltlf& left, const ltlf& right);
    static struct ltlf And(const ltlf& left, const ltlf& right);
    static struct ltlf Until(const ltlf& left, const ltlf& right);
    static struct ltlf Release(const ltlf& left, const ltlf& right);
    static struct ltlf Diamond(const ltlf& sub, bool isGraphGeneration = true);
    static struct ltlf Box(const ltlf& sub, bool isGraphGeneration = true);
    static struct ltlf Last();
    static struct ltlf Implies(const ltlf& left, const ltlf& right, bool isGraphGeneration = true);
    static struct ltlf Equivalent(const ltlf& left, const ltlf& right, bool isGraphGeneration = true);
    static struct ltlf WeakUntil(const ltlf& left, const ltlf& right, bool isForGraphs = true);

    bool easy_interpret(const std::string& map) const;


private:
    //struct ltlf _interpret2(const std::unordered_set<struct ltlf>& map) const;
    // void _allActions(std::unordered_set<std::string> &labels) const;
};


ltlf::ltlf() : casusu{ TRUE }, is_negated{ false } { }

ltlf::ltlf(const std::string& act) : casusu{ ACT }, act{ act }, is_negated{ false } {}

ltlf::ltlf(formula_t citki) : casusu{ citki }, is_negated{ false }   {}

struct ltlf ltlf::True() { return {}; }

struct ltlf ltlf::Act(const std::string& act, bool negated) { ltlf toRet{ act }; toRet.is_negated = negated; return toRet; }

struct ltlf ltlf::Neg(const ltlf& sub) {
    ltlf formula{ NEG_OF };
    formula.args.emplace_back(sub);
    return formula;
}

struct ltlf ltlf::Next(const ltlf& sub) {
    ltlf formula{ NEXT };
    formula.args.emplace_back(sub);
    return formula;
}

struct ltlf ltlf::Or(const ltlf& left, const ltlf& right) {
    struct ltlf formula { OR };
    formula.args.emplace_back(left);
    formula.args.emplace_back(right);
    return formula;
}

struct ltlf ltlf::And(const ltlf& left, const ltlf& right) {
    struct ltlf formula { AND };
    formula.args.emplace_back(left);
    formula.args.emplace_back(right);
    return formula;
}

struct ltlf ltlf::Diamond(const ltlf& sub, bool isForGraphs) {
    if (isForGraphs) return Until(ltlf::True(), sub);
    ltlf formula{ DIAMOND };
    formula.args.emplace_back(sub);
    return formula;
}

struct ltlf ltlf::Box(const ltlf& sub, bool isForGraphs) {
    if (isForGraphs) return  Release({ FALSE }, sub);
    ltlf formula{ BOX };
    formula.args.emplace_back(sub);
    return formula;
}

struct ltlf ltlf::Last() {
    return Neg(Next(True()));
}

struct ltlf ltlf::Until(const ltlf& left, const ltlf& right) {
    struct ltlf formula { UNTIL };
    formula.args.emplace_back(left);
    formula.args.emplace_back(right);
    return formula;
}

struct ltlf ltlf::Release(const ltlf& left, const ltlf& right) {
    struct ltlf formula { RELEASE };
    formula.args.emplace_back(left);
    formula.args.emplace_back(right);
    return formula;
}

struct ltlf ltlf::Implies(const ltlf& left, const ltlf& right, bool isGraphGeneration) {
    if (isGraphGeneration) {
        return Or(Neg(left), right);
    }
    else {
        auto res = Or(Neg(left), And(left, right));
        return res;
    }
}

struct ltlf ltlf::Equivalent(const ltlf& left, const ltlf& right, bool isForGraphs) {
    return And(Implies(left, right, isForGraphs), Implies(right, left, isForGraphs));
}

struct ltlf ltlf::WeakUntil(const ltlf& left, const ltlf& right, bool isForGraphs) {
    return Or(Until(left, right), Box(left, isForGraphs));
}


std::ostream& operator<<(std::ostream& os, const ltlf& syntax) {
    std::string reset = "";
    if (syntax.is_negated)
        os << "!";
    switch (syntax.casusu) {
    case ACT:
        return os << syntax.act << reset;
    case NEG_OF:
        return os << "(!(" << syntax.args[0] << "))" << reset;
    case OR:
        return os << "(" << syntax.args[0] << ") | (" << syntax.args[1] << ')' << reset;
    case AND:
        return os << "(" << syntax.args[0] << ") & (" << syntax.args[1] << ')' << reset;
    case NEXT:
        return os << "X(" << syntax.args[0] << ")" << reset;
    case UNTIL:
        return os << "(" << syntax.args[0] << ") U (" << syntax.args[1] << ')' << reset;
    case RELEASE:
        return os << "(" << syntax.args[0] << ") R (" << syntax.args[1] << ')' << reset;
    case TRUE:
        return os << "true" << reset;
    case BOX:
        return os << "G(" << syntax.args[0] << ")" << reset;
    case DIAMOND:
        return os << "F(" << syntax.args[0] << ")" << reset;
    default:
        return os << "false" << reset;
    }
}


bool ltlf::easy_interpret(const std::string& map) const {
    switch (casusu) {
    case TRUE:
        return true;
    case FALSE:
        return false;
    case ACT:
        if (!is_negated) {
            bool res = (map == (act));
            return res;
        }
        else {
            bool res = !(map == (act));
            return res;
        }
    case NEG_OF:
        return !args.at(0).easy_interpret(map);
    case OR: {
        return args.at(0).easy_interpret(map) || args.at(1).easy_interpret(map);
    }
    case AND: {
        return args.at(0).easy_interpret(map) && args.at(1).easy_interpret(map);
    }
    case NEXT:
        return args.at(0).easy_interpret(map); // Stops the interpretation before the current element
    default:
        assert(false);
    }
}

std::unordered_set<std::string> test_code(const ltlf& formula, std::unordered_set<std::string>& allActions) {
    std::unordered_set<std::string> returned;
    for (const auto& act : allActions) {
        if (formula.easy_interpret(act)) {
            returned.insert(act);
        }
    }
    return returned;
}

//#include <iostream>
//#include <ltlparser/utility.h>


/*ltl_formula* to_aaltaf_rec(const ltlf& formula);

#include <formula/aalta_formula.h>

aalta::aalta_formula* to_aaltaf(const ltlf& formula);*/

//example

//int main() {
//
//    auto B = ltlf::Act("B");
//    auto notB = ltlf::Act("B", true);
//    auto notC = ltlf::Act("C", true);
//    auto C = ltlf::Act("C");
//    auto notD = ltlf::Act("D", true);
//    auto D = ltlf::Act("D");
//    auto f = ltlf::Or(notB, ltlf::And(C, D));
//    std::unordered_set<std::string> allActions{ "A", "B", "C", "D", "E" };
//    auto result = test_code(f, allActions);
//
//    for (const auto& action : result)
//        std::cout << action << std::endl;
//
//    std::cout << "Finish!" << std::endl;
//    return 0;
//}