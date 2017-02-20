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
typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type
put(btrio::formatted_value<F, T> arg, FILE* f) {
    constexpr auto radix = F::get_radix();
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80];
    auto val = arg.value;

    if (maxw == 0) return;

    if (val < 0) {
        std::putc('-', f);
        --maxw;
        --minw;
        val = -val;
    }

    int i = 0;
    do {
        if (maxw == 0) return;
        --maxw;
        if (minw != 0) --minw;
        auto d = val % radix;
        buf[i] = char_map[d];
        val /= radix;
        ++i;
    } while (val && i < sizeof(buf));

    do {
        --i;
        std::putc(buf[i], f);
    } while (i);

    while (minw) {
        std::putc(F::get_fill(), f);
        --minw;
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

template <typename T> auto put(T arg) { return put(arg, stdout); }

}

#endif // PUT_HPP
