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

template <typename T> void put(T arg);

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

template <typename F, typename T>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_signed<T>::value &&
    F::get_pad_side() == btrio::Side::Right >
::type put(btrio::formatted_value<F, T> arg, FILE* f) {
    constexpr auto radix = F::get_radix();
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80];
    auto val = arg.value;

    // print `-` if value is negative
    if (val < 0 && maxw > 0) {
        std::putc('-', f);
        val = -val;
        if (minw != 0) --minw;
        --maxw;
    }

    // buffer stringified value LSD first
    int i = 0;
    while (i < sizeof(buf)) {
        auto d = val % radix;
        buf[i] = char_map[d];
        val /= radix;
        ++i;
        if (val == 0) break;
    }

    // print leading zeros for padding
    for (auto count = i; count <= F::get_padding_zeros() && maxw > 0; ++count) {
        std::putc('0', f);
        if (minw != 0) --minw;
        --maxw;
    }

    // print buffered  value
    while (i > 0 && maxw > 0) {
        --i;
        std::putc(buf[i], f);
        if (minw != 0) --minw;
        --maxw;
    }

    // print padding
    for (minw; minw > 0; --minw) {
        // note that minw <= maxw must be true
        std::putc(F::get_fill(), f);
    }
}

template <typename F, typename T>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_signed<T>::value &&
    F::get_pad_side() == btrio::Side::Left >
::type put(btrio::formatted_value<F, T> arg, FILE* f) {
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
    int i = 0;
    while (i < sizeof(buf)) {
        auto d = val % radix;
        buf[i] = char_map[d];
        val /= radix;
        ++i;
        if (val == 0) break;
    }

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
        std::putc(F::get_fill(), f);
        --maxw;
    }

    // print buffered value
    while (i > 0 && maxw > 0) {
        --i;
        std::putc(buf[i], f);
        --maxw;
    }
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
