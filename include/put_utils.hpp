/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUT_UTILS_HPP
#define PUT_UTILS_HPP

#include "format.hpp"

namespace btrio {

static const char char_map[btrio::default_format::max_radix] = {
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    'a',
    'b',
    'c',
    'd',
    'e',
    'f'
};

template <unsigned int radix, typename OutputIterator, typename T>
OutputIterator itoa_lsdfirst(OutputIterator begin, OutputIterator end, T arg) {

    static_assert( btrio::default_format::min_radix <= radix && radix <= btrio::default_format::max_radix,
                   "Radix is outside the supported range [2,16].");
    static_assert( std::is_integral<T>::value,
                   "Can only cal"
                   "l btrio::itoa_lsdfirst on an integral type.");

    auto val = arg;
    auto cursor = begin;

    while (cursor != end) {
        auto d = val % radix;
        *cursor = char_map[d];
        val /= radix;
        ++cursor;
        if (val == 0) break;
    }

    return cursor;
}

}

#endif // PUT_UTILS_HPP
