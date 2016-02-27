//
// Created by radimir on 27.02.16.
//

#include "util.h"
#include <ostream>
#include <iostream>

std::ostream *debug_stream;


void release() {
    for (auto it = vars.begin(); it != vars.end(); ++it) {
        delete(it->second);
    }
    for (auto it = expr_links.begin(); it != expr_links.end(); ++it) {
        delete(it->second);
    }
    vars.clear();
    expr_links.clear();
}

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

void set_debug(std::ostream* stream) {
    debug_stream = stream;
}

std::ostream &debug() {
    return *debug_stream;
}

bool has_debug() {
    return debug_stream != nullptr;
}