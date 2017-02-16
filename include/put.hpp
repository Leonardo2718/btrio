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

/*template <typename T>
void put(T arg);

template <>
void put(int arg) {
    char buf[16];

    bool neg = arg < 0;
    if (neg) arg = -arg;

    int i = 0;
    do {
        auto d = arg % 10;
        buf[i] = '0' + d;
        arg /= 10;
        ++i;
    } while (arg && i < sizeof(buf));

    if (neg) {
        std::putchar('-');
    }

    do {
        --i;
        std::putchar(buf[i]);
    } while (i);
}

template <>
void put(unsigned int arg) {
    char buf[16];

    int i = 0;
    do {
        auto d = arg % 10;
        buf[i] = '0' + d;
        arg /= 10;
        ++i;
    } while (arg && i < sizeof(buf));

    do {
        --i;
        std::putchar(buf[i]);
    } while (i);
}*/

static const char char_map[] = {
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
typename std::enable_if<std::is_integral<T>::value && std::is_signed<T>::value>::type put(T arg) {
    char buf[20];

    bool neg = arg < 0;
    if (neg) arg = -arg;

    int i = 0;
    do {
        auto d = arg % 10;
        buf[i] = '0' + d;
        arg /= 10;
        ++i;
    } while (arg && i < sizeof(buf));

    if (neg) {
        std::putchar('-');
    }

    do {
        --i;
        std::putchar(buf[i]);
    } while (i);
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value && std::is_unsigned<T>::value>::type put(T arg) {
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
        std::putchar(buf[i]);
    } while (i);
}

template <>
void put(char arg) {
    putchar(arg);
}

void put(const char* arg) {
    for (auto c = arg; *c != 0; ++c) {
        putchar(*c);
    }
}

void put(const void* arg) {
    char buf[16];
    auto v = reinterpret_cast<std::intptr_t>(arg);

    int i = 0;
    do {
        auto d = v % 16;
        buf[i] = char_map[d];
        v >>= 4;/// same as v /= 16
        ++i;
    } while (v && i < sizeof(buf));

    std::putchar('0');
    std::putchar('x');

    do {
        --i;
        std::putchar(buf[i]);
    } while (i);
}

}

#endif // PUT_HPP
