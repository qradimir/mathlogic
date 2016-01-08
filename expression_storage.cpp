//
// Created by radimir on 05.01.16.
//

#include "expression_storage.h"
#include "expression.h"

static _impl EMPTY_STORAGE{0, new expression const*[0]};


expression_storage::expression_storage(size_t count, expression const **items) {
    impl = new _impl(count, items);
    impl->bind();
}

expression_storage::~expression_storage() {
    impl->unbind();
}

expression_storage::expression_storage(expression_storage const &other)
    : impl(other.impl)
{
    impl->bind();
}

_impl::_impl(size_t count, expression const **items)
    : count(count), items(items), usages(0)
{
}

_impl::~_impl() {
    for (int i = 0; i < count; ++i) {
        delete(items[i]);
    }
    delete[](items);
}


expression_storage::expression_storage(expression const* item1)
{
    expression const** items = new expression const*[1];
    items[0] = item1;
    impl = new _impl(1, items);
    impl->bind();
}

expression_storage::expression_storage(expression const *item1, expression const *item2) {
    expression const** items = new expression const*[2];
    items[0] = item1;
    items[1] = item2;
    impl = new _impl(2, items);
    impl->bind();
}

expression_storage::expression_storage() {
    impl = &EMPTY_STORAGE;
    impl->bind();
}

expression_storage &expression_storage::operator=(expression_storage const& other) {
    impl->unbind();
    impl = other.impl;
    impl->bind();
    return *this;
}

bool expression_storage::operator==(expression_storage const &other) const {
    return *impl == *other.impl;
}

bool _impl::operator==(_impl const &other) {
    if (this == &other) return true;

    if (count != other.count) return false;

    for (int i = 0; i < count; ++i) {
        if (!(*items[i] == *other.items[i])) {
            return false;
        }
    }
    return true;
}
