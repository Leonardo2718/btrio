/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#include <string>

#include "format.hpp"
#include "btrio.hpp"

int main() {
    using df = btrio::default_format;
    using Side = btrio::Side;

    btrio::printf("Hello World!\n");

    btrio::printf("Too few arguments: %_ %_\n", -10);

    btrio::printf("Too many arguments: %_ %_\n", -10, 13, 15);

    btrio::printf("A string \"%_\"\n", "inside a string");

    btrio::printf("A std::string \"%_\"\n", std::string{"inside a string"});

    btrio::printf("A percent at the end of a string %", 2);
    btrio::printf(" ... works!\n");

    btrio::printf("%_ is a pointer\n", reinterpret_cast<void*>(0xabcd));

    btrio::print_format(df{});

    auto fval = df::radix<8>::maxw<12>::format(1);
    btrio::printf("value = %_\n", fval.value);
    btrio::print_format<decltype(fval)::format>();

    btrio::printf("A formated number [%_]\n", df::minw<5>::radix<16>::format(255));
    btrio::printf("A formated number [%_]\n", df::minw<5>::radix<16>::format(-255));
    btrio::printf("A formated number [%_]\n", df::minw<5>::radix<16>::fill<'#'>::format(255));
    btrio::printf("A formated number [%_]\n", df::minw<5>::radix<16>::fill<'#'>::format(-255));

    btrio::printf("A formated number [%_]\n", df::minw<5>::pad_side<Side::Right>::radix<16>::format(255));
    btrio::printf("A formated number [%_]\n", df::minw<5>::pad_side<Side::Right>::radix<16>::format(-255));

    btrio::printf("A formated number [%_]\n", df::minw<5>::pad_side<Side::Right>::fill<'#'>::radix<16>::format(255));
    btrio::printf("A formated number [%_]\n", df::minw<5>::pad_side<Side::Right>::fill<'#'>::radix<16>::format(-255));

    for (int i = 0; i < 16; i++) {
        btrio::printf("%_ ", df::radix<4>::padding_zeros<3>::pad_side<Side::Right>::format(i));
    }
    btrio::printf("\n");

    btrio::printf("Another formatted number [%_]\n", df::setw<6>::fill<'#'>::padding_zeros<2>::format(-1));
    btrio::printf("Another formatted number [%_]\n", df::setw<6>::fill<'#'>::padding_zeros<2>::pad_side<Side::Right>::format(-1));
}
