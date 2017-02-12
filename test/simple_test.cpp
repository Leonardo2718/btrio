/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#include "btrio.hpp"

#include <string>

int main() {
    auto s = std::string{"Hello World! %_ %_\n"};
    btrio::iprintf(s.cbegin(), s.cend(), -10, 13);
}
