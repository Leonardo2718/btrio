/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef FORMAT_HPP
#define FORMAT_HPP

#define FORMAT_ARGS _radix, _decimals
#define FROMAT_TYPES unsigned int, unsigned int
#define FORMAT_TARGS unsigned int _radix, unsigned int _decimals
#define FORMAT_DEFAULT_TARGS unsigned int _radix = 10, unsigned int _decimals = 3

#define FORMAT_TEMPLATE template <FORMAT_TARGS>
#define FORMAT_DEFAULT_TEMPLATE template<FORMAT_DEFAULT_TARGS>

namespace btrio {

FORMAT_DEFAULT_TEMPLATE struct static_format;

using df = static_format<>;

}

FORMAT_TEMPLATE
struct btrio::static_format {
        static_assert(_radix >= 2 && _radix <= 16, "Radix is outside the supported range [2,16].");

        static constexpr auto get_radix() { return _radix; }
        static constexpr auto get_decimals() { return _decimals; }

        template <unsigned int r>
        using radix = static_format<r, _decimals>;

        template <unsigned int d>
        using decc = static_format<_radix, d>;
};

#endif // FORMAT_HPP
