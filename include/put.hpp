/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUT_HPP
#define PUT_HPP

#include "format.hpp"
#include "put_utils.hpp"
#include "basic_put.hpp"

#include <cstdio>

#define USE_PPUT 0

namespace btrio {

//~ iput ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename OutputIterator, typename T, typename F = btrio::default_format>
OutputIterator iput(OutputIterator begin, OutputIterator end, T arg) {
    auto cursor = begin;
    return basic_put<F>(arg, [&](auto c){ *cursor = c; ++cursor; return cursor; }, [&](auto i){ return i == end; }, cursor);
}

template <typename OutputIterator, typename T, typename F>
OutputIterator iput(OutputIterator begin, OutputIterator end, btrio::formatted_value<F, T> arg) {
    auto cursor = begin;
    return basic_put<F>(arg.value, [&](auto c){ *cursor = c; ++cursor; return cursor; }, [&](auto i){ return i == end; }, cursor);
}

//~ fput ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T, typename F = btrio::default_format>
auto fput(T&& arg, FILE* f) {
    return basic_put<F>(std::forward<T>(arg), std::putc, [](auto r){ return r == EOF; }, 0, f);
}

template <typename T, typename F>
auto fput(btrio::formatted_value<F, T> arg, FILE* f) {
    return basic_put<F>(arg.value, std::putc, [](auto r){ return r == EOF; }, 0, f);
}

//~ put ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T> void put(T arg) { put(arg, stdout); }

}

#endif // PUT_HPP
