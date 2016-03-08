//
// Created by radimir on 26.02.16.
//

#ifndef MATHLOGIC_UTIL_H
#define MATHLOGIC_UTIL_H

#include <functional>
#include <map>
#include <ctime>
#include <string.h>

#define DEBUG(d) if (has_debug()) { debug() << d << '\n'; }

#define INIT_TIME clock_t _time = clock();
#define LOG_TIME_DELTA(str) DEBUG((double)(clock() - _time) / CLOCKS_PER_SEC << (str)); _time = clock();

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

template <typename T>
class no_value_exception;

template <typename T>
struct evalue {
    typedef std::map<std::string, T> holder;
    typedef no_value_exception<T> no_value;
};

template<typename T>
class no_value_exception : public std::exception {
    std::string error_msg;
public:
    typename evalue<T>::holder const err_holder;
    std::string miss_value_name;


    no_value_exception(const typename evalue<T>::holder &err_holder, std::string miss_value_name)
            : err_holder(err_holder),
              miss_value_name(miss_value_name),
              error_msg("no expected value '" + miss_value_name + "' in value holder")
    { }


    virtual const char *what() const throw() {
        return error_msg.c_str();
    }
};



void set_debug(bool);
std::ostream& debug();
bool has_debug();

#endif //MATHLOGIC_UTIL_H