/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BASIC_PUT_HPP
#define BASIC_PUT_HPP

#include "format.hpp"
#include "put_utils.hpp"

#include <type_traits>
#include <utility>
#include <cstdint>
#include <cstring>

namespace btrio {

template <typename F, typename T, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_signed<T>::value &&
    !std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, char>::value && // do not handle char as a number type
    F::get_pad_side() == btrio::Side::Right,
    typename std::remove_reference<Ret>::type >
::type basic_put(T arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    constexpr auto radix = F::get_radix();
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80] = {0};
    auto val = arg;

    // print `-` if value is negative
    if (val < 0 && maxw > 0) {
        val = -val;
        auto r =  putc('-', putcArgs...);
        if (predicate(r)) return r;
        if (minw != 0) --minw;
        --maxw;
    }

    // buffer stringified value LSD first
    auto i = btrio::itoa_lsdfirst<radix>(std::begin(buf), std::end(buf), val) - std::cbegin(buf);

    // print leading zeros for padding
    for (auto count = i; count <= F::get_padding_zeros() && maxw > 0; ++count) {
        auto r = putc('0', putcArgs...);
        if (predicate(r)) return r;
        if (minw != 0) --minw;
        --maxw;
    }

    // print buffered  value
    while (i > 0 && maxw > 0) {
        --i;
        auto r = putc(buf[i], putcArgs...);
        if (predicate(r)) return r;
        if (minw != 0) --minw;
        --maxw;
    }

    // print padding
    for (minw; minw > 0; --minw) {
        // note that minw <= maxw must be true
        auto r = putc(F::get_fill(), putcArgs...);
        if (predicate(r)) return r;
    }

    return default_return;
}

template <typename F, typename T, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_signed<T>::value &&
    !std::is_same<std::remove_cv_t<std::remove_reference_t<T>>, char>::value && // do not handle char as a number type
    F::get_pad_side() == btrio::Side::Left,
    typename std::remove_reference<Ret>::type >
::type basic_put(T arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    constexpr auto radix = F::get_radix();
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80] = {0};
    auto val = arg;

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
        auto r = putc(F::get_fill(), putcArgs...);
        if (predicate(r)) return r;
        --maxw;
    }

    // print buffered value
    while (i > 0 && maxw > 0) {
        --i;
        auto r = putc(buf[i], putcArgs...);
        if (predicate(r)) return r;
        --maxw;
    }

    return default_return;
}

template <typename F, typename T, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_unsigned<T>::value &&
    F::get_pad_side() == btrio::Side::Right,
    typename std::remove_reference<Ret>::type >
::type basic_put(T arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    constexpr auto radix = F::get_radix();
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80] = {0};
    auto val = arg;

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
        auto r = putc(buf[i], putcArgs...);
        if (predicate(r)) return r;
        if (minw != 0) --minw;
        --maxw;
    }

    // print padding
    for (minw; minw > 0; --minw) {
        // note that minw <= maxw must be true
        auto r = putc(F::get_fill(), putcArgs...);
        if (predicate(r)) return r;
    }

    return default_return;
}

template <typename F, typename T, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
typename std::enable_if<
    std::is_integral<T>::value &&
    std::is_unsigned<T>::value &&
    F::get_pad_side() == btrio::Side::Left,
    typename std::remove_reference<Ret>::type >
::type basic_put(T arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    constexpr auto radix = F::get_radix();
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80] = {0};
    auto val = arg;

    // buffer stringified value LSD first
    auto i = btrio::itoa_lsdfirst<radix>(std::begin(buf), std::end(buf), val) - std::cbegin(buf);

    // buffer leading zeros for padding
    for (i; i <= F::get_padding_zeros() && i < sizeof(buf); ++i) {
        buf[i] = '0';
    }

    // print padding
    for (minw; minw > i; --minw) {
        // note that minw <= maxw must be true
        auto r = putc(F::get_fill(), putcArgs...);
        if (predicate(r)) return r;
        --maxw;
    }

    // print buffered value
    while (i > 0 && maxw > 0) {
        --i;
        auto r = putc(buf[i], putcArgs...);
        if (predicate(r)) return r;
        --maxw;
    }

    return default_return;
}

template <typename F, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
std::enable_if_t<F::get_pad_side() == btrio::Side::Right, typename std::remove_reference<Ret>::type >
basic_put(const char arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    constexpr auto maxw = F::get_maxw();
    constexpr auto minw = F::get_minw();

    // print padding
    for (auto i = 0; i < minw - 1; ++i) { // allow room for the single character
        // note that minw <= maxw must be true
        auto r = putc(F::get_fill(), putcArgs...);
        if (predicate(r)) return r;
    }

    // print the single character if there is room for it
    if (maxw > 0) {
        // note that minw <= maxw must be true
        auto r = putc(arg, putcArgs...);
        if (predicate(r)) return r;
    }

    return default_return;
}

template <typename F, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
std::enable_if_t<F::get_pad_side() == btrio::Side::Left, typename std::remove_reference<Ret>::type >
basic_put(const char arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    constexpr auto maxw = F::get_maxw();
    constexpr auto minw = F::get_minw();

    if (maxw > 0) {
        // note that minw <= maxw must be true
        auto r = putc(arg, putcArgs...);
        if (predicate(r)) return r;
    }

    // print padding
    for (auto i = 1; i < minw; ++i) {
        auto r = putc(F::get_fill(), putcArgs...);
        if (predicate(r)) return r;
    }

    return default_return;
}

template <typename F, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
typename std::enable_if<F::get_pad_side() == btrio::Side::Right, typename std::remove_reference<Ret>::type >::type
basic_put(const void* arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80] = {0};
    auto val = reinterpret_cast<uintptr_t>(arg);

    // buffer stringified value LSD first
    auto i = btrio::itoa_lsdfirst<16>(std::begin(buf), std::end(buf), val) - std::cbegin(buf);

    // print leading `0x`
    if (maxw > 0) {
        putc('0', putcArgs...);
        if (minw != 0) --minw;
        --maxw;
    }
    if (maxw > 0) {
        putc('x', putcArgs...);
        if (minw != 0) --minw;
        --maxw;
    }

    // print leading zeros for padding
    for (auto count = i; count <= F::get_padding_zeros() && maxw > 0; ++count) {
        putc('0', putcArgs...);
        if (minw != 0) --minw;
        --maxw;
    }

    // print buffered  value
    while (i > 0 && maxw > 0) {
        --i;
        auto r = putc(buf[i], putcArgs...);
        if (predicate(r)) return r;
        if (minw != 0) --minw;
        --maxw;
    }

    // print padding
    for (minw; minw > 0; --minw) {
        // note that minw <= maxw must be true
        auto r = putc(F::get_fill(), putcArgs...);
        if (predicate(r)) return r;
    }

    return default_return;
}

template <typename F, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
typename std::enable_if<F::get_pad_side() == btrio::Side::Left, typename std::remove_reference<Ret>::type >::type
basic_put(const void* arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    auto minw = F::get_minw();
    auto maxw = F::get_maxw();
    char buf[80] = {0};
    auto val = reinterpret_cast<uintptr_t>(arg);

    // buffer stringified value LSD first
    auto i = btrio::itoa_lsdfirst<16>(std::begin(buf), std::end(buf), val) - std::cbegin(buf);

    // buffer leading zeros for padding
    for (i; i <= F::get_padding_zeros() && i < sizeof(buf); ++i) {
        buf[i] = '0';
    }

    // print padding
    for (minw; minw > i + 2; --minw) { // allow space for `0x`
        // note that minw <= maxw must be true
        auto r = putc(F::get_fill(), putcArgs...);
        if (predicate(r)) return r;
        --maxw;
    }

    // print leading `0x`
    if (maxw > 0) {
        putc('0', putcArgs...);
        if (minw != 0) --minw;
        --maxw;
    }
    if (maxw > 0) {
        putc('x', putcArgs...);
        if (minw != 0) --minw;
        --maxw;
    }

    // print buffered value
    while (i > 0 && maxw > 0) {
        --i;
        auto r = putc(buf[i], putcArgs...);
        if (predicate(r)) return r;
        --maxw;
    }

    return default_return;
}

template <typename F, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
std::enable_if_t<F::get_pad_side() == btrio::Side::Right, typename std::remove_reference<Ret>::type >
basic_put(const char* arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    constexpr auto minw = F::get_minw();
    constexpr auto maxw = F::get_maxw();
    auto count = 0;

    // print the litteral string
    for (auto c = arg; *c != 0 && count < maxw; ++c, ++count) {
        auto r = putc(*c, putcArgs...);
        if (predicate(r)) return r;
    }

    // print padding
    for (count; count < minw; ++count) {
        // note that minw <= maxw must be true
        auto r = putc(F::get_fill(), putcArgs...);
        if (predicate(r)) return r;
    }

    return default_return;
}

template <typename F, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
std::enable_if_t<F::get_pad_side() == btrio::Side::Left, typename std::remove_reference<Ret>::type >
basic_put(const char* arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    constexpr auto minw = F::get_minw();
    constexpr auto maxw = F::get_maxw();
    const auto len = std::strlen(arg);
    auto count = 0;

    // print padding
    for (auto i = 0; i + len < minw; ++i, ++count) {
        // note that minw <= maxw must be true
        auto r = putc(F::get_fill(), putcArgs...);
        if (predicate(r)) return r;
    }

    // print the litteral string
    for (auto c = arg; *c != 0 && count < maxw; ++c, ++count) {
        auto r = putc(*c, putcArgs...);
        if (predicate(r)) return r;
    }

    return default_return;
}

template <typename F, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
auto basic_put(const std::string& arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    return basic_put(F::format(arg.c_str()), putc, predicate, default_return, putcArgs...);
}

template <typename F, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
auto basic_put(btrio::Side arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    using S = btrio::Side;
    switch (arg) {
        case S::Left: return basic_put<F>(std::string{"Left"}, putc, predicate, std::forward<Ret>(default_return), putcArgs...);
        case S::Right: return basic_put<F>(std::string{"Right"}, putc, predicate, std::forward<Ret>(default_return), putcArgs...);
    }
}

template <typename F, typename T, typename Putc, typename Pred, typename Ret, typename... PutcArgs>
auto basic_put(btrio::formatted_value<F, T> arg, Putc putc, Pred predicate, Ret&& default_return, PutcArgs... putcArgs) {
    return basic_put<F>(arg.value, putc, predicate, default_return, putcArgs...);
}

}

#endif // BASIC_PUT_HPP
