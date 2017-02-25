/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PUT_HPP
#define PUT_HPP

#include <type_traits>
#include <cstdio>
#include <cinttypes>

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

template <typename T> void put(T arg);

template <typename OutputIterator, typename F, typename T>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_signed<T>::value &&
    F::get_pad_side() == btrio::Side::Right,
    OutputIterator>
::type iput(OutputIterator begin, OutputIterator end, btrio::formatted_value<F, T> arg) {
    constexpr auto radix = F::get_radix();
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80] = {0};
    auto val = arg.value;
    auto cursor = begin;

    // print `-` if value is negative
    if (val < 0 && maxw > 0) {
        val = -val;
        *cursor = '-';
        ++cursor;
        if (cursor == end) return cursor;
        if (minw != 0) --minw;
        --maxw;
    }

    // buffer stringified value LSD first
    auto i = btrio::itoa_lsdfirst<radix>(std::begin(buf), std::end(buf), val) - std::cbegin(buf);

    // print leading zeros for padding
    for (auto count = i; count <= F::get_padding_zeros() && maxw > 0; ++count) {
        *cursor = '0';
        ++cursor;
        if (cursor == end) return cursor;
        if (minw != 0) --minw;
        --maxw;
    }

    // print buffered  value
    while (i > 0 && maxw > 0) {
        --i;
        *cursor = buf[i];
        ++cursor;
        if (cursor == end) return cursor;
        if (minw != 0) --minw;
        --maxw;
    }

    // print padding
    for (minw; minw > 0; --minw) {
        // note that minw <= maxw must be true
        *cursor = F::get_fill();
        ++cursor;
        if (cursor == end) return cursor;
    }

    return cursor;
}

template <typename OutputIterator, typename F, typename T>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_signed<T>::value &&
    F::get_pad_side() == btrio::Side::Left,
    OutputIterator>
::type iput(OutputIterator begin, OutputIterator end, btrio::formatted_value<F, T> arg) {
    constexpr auto radix = F::get_radix();
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80];
    auto val = arg.value;
    auto cursor = begin;

    // save whether value is negative
    auto neg = val < 0;
    if (neg) {
        val = -val;
    }

    // buffer stringified value LSD first
    auto i = btrio::itoa_lsdfirst<radix>(std::begin(buf), std::end(buf), val) - std::cbegin(buf);

    // buffer leading zeros for padding
    for (i; i <= F::get_padding_zeros() && i < sizeof(buf); ++i) {
        buf[i] = '0';
    }

    // buffer minus sign
    if (neg) {
        buf[i] = '-';
        ++i;
    }

    // print padding
    for (minw; minw > i; --minw) {
        // note that minw <= maxw must be true
        *cursor = F::get_fill();
        ++cursor;
        if (cursor == end) return cursor;
        --maxw;
    }

    // print buffered value
    while (i > 0 && maxw > 0) {
        --i;
        *cursor = buf[i];
        ++cursor;
        if (cursor == end) return cursor;
        --maxw;
    }

    return cursor;
}


template <typename F, typename T, typename Putc, typename... PutcArgs>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_signed<T>::value &&
    F::get_pad_side() == btrio::Side::Right >
::type pput(btrio::formatted_value<F, T> arg, Putc putc, PutcArgs... putcArgs) {
    constexpr auto radix = F::get_radix();
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80];
    auto val = arg.value;

    // print `-` if value is negative
    if (val < 0 && maxw > 0) {
        val = -val;
        putc('-', putcArgs...);
        if (minw != 0) --minw;
        --maxw;
    }

    // buffer stringified value LSD first
    auto i = btrio::itoa_lsdfirst<radix>(std::begin(buf), std::end(buf), val) - std::cbegin(buf);

    // print leading zeros for padding
    for (auto count = i; count <= F::get_padding_zeros() && maxw > 0; ++count) {
        putc('0', putcArgs...);
        if (minw != 0) --minw;
        --maxw;
    }

    // print buffered  value
    while (i > 0 && maxw > 0) {
        --i;
        putc(buf[i], putcArgs...);
        if (minw != 0) --minw;
        --maxw;
    }

    // print padding
    for (minw; minw > 0; --minw) {
        // note that minw <= maxw must be true
        putc(F::get_fill(), putcArgs...);
    }
}

template <typename Putc, typename F, typename T, typename... PutcArgs>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_signed<T>::value &&
    F::get_pad_side() == btrio::Side::Left >
::type pput(btrio::formatted_value<F, T> arg, Putc putc, PutcArgs... putcArgs) {
    constexpr auto radix = F::get_radix();
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80];
    auto val = arg.value;

    // save whether value is negative
    auto neg = val < 0;
    if (neg) {
        val = -val;
    }

    // buffer stringified value LSD first
    auto i = btrio::itoa_lsdfirst<radix>(std::begin(buf), std::end(buf), val) - std::cbegin(buf);

    // buffer leading zeros for padding
    for (i; i <= F::get_padding_zeros() && i < sizeof(buf); ++i) {
        buf[i] = '0';
    }

    // buffer minus sign
    if (neg) {
        buf[i] = '-';
        ++i;
    }

    // print padding
    for (minw; minw > i; --minw) {
        // note that minw <= maxw must be true
        putc(F::get_fill(), putcArgs...);
        --maxw;
    }

    // print buffered value
    while (i > 0 && maxw > 0) {
        --i;
        putc(buf[i], putcArgs...);
        --maxw;
    }
}


template <typename F, typename T>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_signed<T>::value &&
    F::get_pad_side() == btrio::Side::Right >
::type put(btrio::formatted_value<F, T> arg, FILE* f) {
    char buf[80];
    auto end = iput(std::begin(buf), std::end(buf), arg);

    for (auto cursor = std::begin(buf); cursor != end; ++cursor) {
        std::putc(*cursor, f);
    }
}

template <typename F, typename T>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_signed<T>::value &&
    F::get_pad_side() == btrio::Side::Left >
::type put(btrio::formatted_value<F, T> arg, FILE* f) {
    char buf[80];
    auto end = iput(std::begin(buf), std::end(buf), arg);

    for (auto cursor = std::begin(buf); cursor != end; ++cursor) {
        std::putc(*cursor, f);
    }
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type put(T arg, FILE* f) {
    char buf[20];

    bool neg = arg < 0;
    if (neg) {
        std::putc('-', f);
        arg = -arg;
    }

    int i = 0;
    do {
        auto d = arg % 10;
        buf[i] = '0' + d;
        arg /= 10;
        ++i;
    } while (arg && i < sizeof(buf));

    do {
        --i;
        std::putc(buf[i], f);
    } while (i);
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value>::type put(T arg, FILE* f) {
    char buf[20];

    int i = 0;
    do {
        auto d = arg % 10;
        buf[i] = '0' + d;
        arg /= 10;
        ++i;
    } while (arg && i < sizeof(buf));

    do {
        --i;
        std::putc(buf[i], f);
    } while (i);
}

void put(const char arg, FILE* f) {
    std::putc(arg, f);
}

void put(const char* arg, FILE* f) {
    for (auto c = arg; *c != 0; ++c) {
        std::putc(*c, f);
    }
}

void put(const std::string& arg, FILE* f) {
    for (auto c : arg) {
        std::putc(c, f);
    }
}

void put(const void* arg, FILE* f) {
    char buf[16];
    auto v = reinterpret_cast<std::intptr_t>(arg);

    int i = 0;
    do {
        auto d = v % 16;
        buf[i] = char_map[d];
        v >>= 4; // same as v /= 16
        ++i;
    } while (v && i < sizeof(buf));

    std::putc('0', f);
    std::putc('x', f);

    do {
        --i;
        std::putc(buf[i], f);
    } while (i);
}

void put(const btrio::Side& s, FILE* f) {
    using S = btrio::Side;
    switch (s) {
        case S::Left:
            put(std::string{"Left"}, f);
            break;
        case S::Right:
            put(std::string{"Right"}, f);
            break;
    }
}

template <typename T> void put(T arg) { put(arg, stdout); }

}

#endif // PUT_HPP
