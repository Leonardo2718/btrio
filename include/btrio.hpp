/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BTRIO_HPP
#define BTRIO_HPP

#include "format.hpp"
#include "put.hpp"

#include <cstdio>
#include <utility>

namespace btrio {

template <typename InputIterator, typename T, typename... Ts>
void ifprintf(FILE* f, InputIterator begin, InputIterator end, T arg, Ts... args);

template <typename InputIterator>
void ifprintf(FILE* f, InputIterator begin, InputIterator end);

template <typename InputIterator, typename... Ts>
void iprintf(InputIterator begin, InputIterator end, Ts... args) {
    ifprintf(stdout, begin, end, args...);
}

template <typename InputRange, typename... Ts>
void printf(InputRange r, Ts&&... args) {
    iprintf(std::cbegin(r), std::cend(r), args...);
}

template <typename... Ts>
void printf(const char* str, Ts&&... args) {
    printf(std::string{str}, args...);
}

template <typename F>
void print_format() {
    btrio::printf("radix = %_, decimals = %_\n", F::get_radix(), F::get_decimals());
}

FORMAT_TEMPLATE
void print_format(btrio::static_format<FORMAT_ARGS> f) {
    btrio::printf("radix = %_, decimals = %_\n", _radix, _decimals);
}

}

template <typename InputIterator, typename T, typename... Ts>
void btrio::ifprintf(FILE* f, InputIterator begin, InputIterator end, T arg, Ts... args) {
    auto cursor = begin;

    while (cursor != end) {
        if (*cursor == '%') {
            ++cursor;
            if (*cursor == '_') {
                put(arg);
                ++cursor;
                break;
            }
            else {
                std::putc('%', f);
                std::putc(*cursor, f);
            }
        }
        else {
            std::putc(*cursor, f);
        }
        ++cursor;
    }

    iprintf(cursor, end, args...);
}

template <typename InputIterator>
void btrio::ifprintf(FILE* f, InputIterator begin, InputIterator end) {
    for (auto cursor = begin; cursor != end; ++cursor) {
        std::putc(*cursor, f);
    }
}

#endif // BTRIO_HPP
