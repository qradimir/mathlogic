//
// Created by radimir on 05.01.16.
//

#ifndef MATHLOGIC_EXPRESSION_STORAGE_H
#define MATHLOGIC_EXPRESSION_STORAGE_H


#include <stddef.h>
#include <assert.h>

class expression;

class _impl {
public:
    expression const** items;
    size_t count, usages;

    _impl(size_t count, expression const **items);
    ~_impl();

    inline void bind() {
        usages++;
    }
    inline void unbind() {
        usages--;
        if (usages == 0) {
            delete this;
        }
    }

    bool operator==(_impl const& other);
};

class expression_storage {

    _impl * impl;

public:

    expression_storage();

    expression_storage(size_t count, expression const **items);
    expression_storage(expression_storage const& other);

    expression_storage(expression const* item1);
    expression_storage(expression const* item1, expression const* item2);


    ~expression_storage();

    expression_storage& operator=(expression_storage const& other);

    bool operator==(expression_storage const& other) const;

    inline expression const* get(size_t i) const {
        assert(i < impl->count);
        return  impl->items[i];
    }

    inline expression const* operator[](size_t i) const {
        return get(i);
    }

    inline size_t size() const {
        return impl->count;
    }

};

#endif //MATHLOGIC_EXPRESSION_STORAGE_H
