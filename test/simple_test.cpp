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

    btrio::printf("Hello World!\n");

    btrio::printf("Too few arguments: %_ %_\n", -10);

    btrio::printf("Too many arguments: %_ %_\n", -10, 13, 15);

    btrio::printf("A string \"%_\"\n", "inside a string");

    btrio::printf("A percent at the end of a string %", 2);
    btrio::printf(" ... works!\n");

    btrio::printf("%_ is a pointer\n", reinterpret_cast<void*>(0xabcd));

    btrio::print_format(df{});

    using f = df::radix<5>::decimals<15>;
    btrio::print_format<f>();

    auto fval = df::radix<8>::maxw<12>::format(1);
    btrio::printf("%_\n", fval.value);
    btrio::print_format<decltype(fval)::format>();

    btrio::printf("A formated number [%_]\n", df::minw<5>::radix<16>::format(255));
}
