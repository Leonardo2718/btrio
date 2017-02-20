/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PRINT_HPP
#define PRINT_HPP

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

template <typename F> void print_format();

FORMAT_TEMPLATE void print_format(btrio::static_format<FORMAT_ARGS>);

}

template <typename InputIterator, typename T, typename... Ts>
void btrio::ifprintf(FILE* f, InputIterator begin, InputIterator end, T arg, Ts... args) {
    auto cursor = begin;

    while (cursor != end) {
        if (*cursor == '%') {
            ++cursor;
            if (cursor == end) {
                std::putc('%', f);
                break;
            }
            else if (*cursor == '_') {
                put(arg, f);
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

static constexpr auto format_fstring =
    "Format:\n"
    "    radix = %_\n"
    "    decimals = %_\n"
    "    leading zeros = %_\n"
    "    pad side = %_\n"
    "    minw = %_\n"
    "    maxw = %_\n"
    "    fill = '%_'\n"
    ;

template <typename F>
void btrio::print_format() {
    btrio::printf(format_fstring
                  , F::get_radix()
                  , F::get_decimals()
                  , F::get_leading_zeros()
                  , F::get_pad_side()
                  , F::get_minw()
                  , F::get_maxw()
                  , F::get_fill()
                  );
}

FORMAT_TEMPLATE
void btrio::print_format(btrio::static_format<FORMAT_ARGS>) {
    btrio::printf(format_fstring
                  , _radix
                  , _decimals
                  , _leading_zeros
                  , _pad_side
                  , _minw
                  , _maxw
                  , _fill
                  );
}

#endif // PRINT_HPP
