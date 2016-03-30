//
// Created by radimir on 27.02.16.
//

#include "util.h"
#include <ostream>
#include <iostream>
#include <set>

bool d = false;


bool *bit_tuple::do_while(std::function<bool(bool const *, size_t)> f) {
    for (size_t i = 0; i < size; ++i) { bits[i] = false; }
    size_t all = 1u << size;
    for (size_t i = 0; i < all; ++i) {
        if (!f(bits, size)) {
            break;
        }
        next();
    }
    return bits;
}

void set_debug(bool is_debug) {
    d = is_debug;
}

std::ostream &debug() {
    return std::cerr;
}

bool has_debug() {
    return d;
}