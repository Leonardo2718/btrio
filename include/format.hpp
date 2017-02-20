/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef FORMAT_HPP
#define FORMAT_HPP

namespace btrio {

enum class Side {Left, Right};

}

#define FORMAT_ARGS \
    _radix, \
    _decimals, \
    _pad_side, \
    _minw, \
    _maxw, \
    _fill
#define FROMAT_TYPES \
    unsigned int, \
    unsigned int, \
    btrio::Side, \
    unsigned int, \
    unsigned int, \
    char
#define FORMAT_TARGS \
    unsigned int _radix, \
    unsigned int _decimals, \
    btrio::Side _pad_side, \
    unsigned int _minw, \
    unsigned int _maxw, \
    char _fill
#define FORMAT_DEFAULT_TARGS \
    unsigned int _radix = 10, \
    unsigned int _decimals = 3, \
    btrio::Side _pad_side = btrio::Side::Left, \
    unsigned int _minw = 0, \
    unsigned int _maxw = -1, \
    char _fill = ' '

#define FORMAT_TEMPLATE template <FORMAT_TARGS>
#define FORMAT_DEFAULT_TEMPLATE template<FORMAT_DEFAULT_TARGS>

namespace btrio {

FORMAT_DEFAULT_TEMPLATE struct static_format;

using default_format = static_format<>;

template <typename F, typename T> struct formatted_value;

}

FORMAT_TEMPLATE
struct btrio::static_format {
        static constexpr unsigned int min_radix = 2;
        static constexpr unsigned int max_radix = 16;

        static_assert(min_radix <= _radix && _radix <= max_radix, "Radix is outside the supported range [2,16].");
        static_assert(_minw <= _maxw, "Minimum printing margine is greater than the set maximum");

        static constexpr auto get_radix() { return _radix; }
        static constexpr auto get_decimals() { return _decimals; }
        static constexpr auto get_pad_side() { return _pad_side; }
        static constexpr auto get_minw() { return _minw; }
        static constexpr auto get_maxw() { return _maxw; }
        static constexpr auto get_fill() { return _fill; }

        template <unsigned int r>
        using radix = static_format<r, _decimals, _pad_side, _minw, _maxw, _fill>;

        template <unsigned int d>
        using decimals = static_format<_radix, d, _pad_side, _minw, _maxw, _fill>;

        template <btrio::Side s>
        using pad_side = static_format<_radix, _decimals, s, _minw, _maxw, _fill>;

        template <unsigned int w>
        using minw = static_format<_radix, _decimals, _pad_side, w, _maxw, _fill>;

        template <unsigned int w>
        using maxw = static_format<_radix, _decimals, _pad_side, _minw, w, _fill>;

        template <unsigned int w>
        using setw = static_format<_radix, _decimals, _pad_side, w, w, _fill>;

        template <char c>
        using fill = static_format<_radix, _decimals, _pad_side, _minw, _maxw, c>;

        using _this = static_format<FORMAT_ARGS>;

        template <typename T>
        static auto format(T val) { return formatted_value<_this, T>{val}; }
};

template <typename F, typename T> struct btrio::formatted_value {
        using format = F;
        T value;
};

#endif // FORMAT_HPP
