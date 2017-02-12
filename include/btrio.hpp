/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BTRIO_HPP
#define BTRIO_HPP

#include <cstdio>
#include <cstdlib>
#include <memory>

namespace btrio {

template <typename T>
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
}

template <typename InputIterator, typename T, typename... Ts>
void iprintf(InputIterator begin, InputIterator end, T arg, Ts... args);

template <typename InputIterator>
void iprintf(InputIterator begin, InputIterator end);

}

template <typename InputIterator, typename T, typename... Ts>
void btrio::iprintf(InputIterator begin, InputIterator end, T arg, Ts... args) {
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
                std::putchar('%');
                std::putchar(*cursor);
            }
        }
        else {
            std::putchar(*cursor);
        }
        ++cursor;
    }

    iprintf(cursor, end, args...);
}

template <typename InputIterator>
void btrio::iprintf(InputIterator begin, InputIterator end) {
    for (auto cursor = begin; cursor != end; ++cursor) {
        std::putchar(*cursor);
    }
}

#endif // BTRIO_HPP
