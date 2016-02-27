//
// Created by radimir on 26.02.16.
//

#ifndef MATHLOGIC_UTIL_H
#define MATHLOGIC_UTIL_H

#include <functional>
#include <map>

#define DEBUG(d) if (has_debug()) { debug() << d << '\n'; }

#define INIT_TIME clock_t _time = clock();
#define LOG_TIME_DELTA(str) DEBUG((double)(clock() - _time) / CLOCKS_PER_SEC << (str)); _time = clock();


class variable;
class expression_link;

static std::map<std::string, variable*> vars;
static std::map<std::string, expression_link*> expr_links;

class bit_tuple {
    bool* bits;
    size_t size;

    void next() {
        bool *first = bits, *end = first + size;
        for (; bits != end && *bits; ++bits) {
        }
        *bits = true;
        end = bits;
        for (bits = first; bits != end; ++bits) {
            *bits = false;
        }
        bits = first;
    }

public:
    bit_tuple(size_t size) : bits(new bool[size]), size(size) { }

    bool * do_while(std::function<bool(bool const *, size_t)> f);
};

void release();


void set_debug(std::ostream* s);
std::ostream& debug();
bool has_debug();

#endif //MATHLOGIC_UTIL_H