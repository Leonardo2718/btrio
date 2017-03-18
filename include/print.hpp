/**
 *         Copyright Leonardo Banderali 2017 - 2017.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef PRINT_HPP
#define PRINT_HPP

#include "format.hpp"
#include "put.hpp"

namespace btrio {

template <typename OutputIterator, typename InputIterator, typename T, typename... Ts>
void iiprintf(OutputIterator obegin, OutputIterator oend, InputIterator ibegin, InputIterator iend, T arg, Ts... args);

template <typename OutputIterator, typename InputIterator>
void iiprintf(OutputIterator obegin, OutputIterator oend, InputIterator begin, InputIterator end);

template <typename InputIterator, typename T, typename... Ts>
void fiprintf(FILE* f, InputIterator begin, InputIterator end, T arg, Ts... args);

template <typename InputIterator>
void fiprintf(FILE* f, InputIterator begin, InputIterator end);

template <typename InputIterator, typename... Ts>
void iprintf(InputIterator begin, InputIterator end, Ts... args) {
    fiprintf(stdout, begin, end, args...);
}

template <typename InputRange, typename... Ts>
void printf(InputRange r, Ts&&... args) {
    iprintf(std::cbegin(r), std::cend(r), args...);
}

template <typename... Ts>
void printf(const char* str, Ts&&... args) {
    printf(std::string{str}, args...);
}

template <typename F> void print_format();

FORMAT_TEMPLATE void print_format(btrio::static_format<FORMAT_ARGS>);

}

//~ iiprintf ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename OutputIterator, typename InputIterator, typename T, typename... Ts>
void btrio::iiprintf(OutputIterator obegin, OutputIterator oend, InputIterator ibegin, InputIterator iend, T arg, Ts... args) {
    auto icursor = ibegin;
    auto ocursor = obegin;

    while (icursor != iend && ocursor != oend) {
        if (*icursor == '%') {
            ++icursor;
            if (icursor == iend) {
                *ocursor = '%';
                break;
            }
            else if (*icursor == '_') {
                ocursor = iput(ocursor, oend, arg);
                ++icursor;
                break;
            }
            else {
                *ocursor = '%';
                ++ocursor;
                *ocursor = *icursor;
            }
        }
        else {
            *ocursor = *icursor;
        }
        ++icursor;
        ++ocursor;
    }

    iiprintf(ocursor, oend, icursor, iend, args...);
}

template <typename OutputIterator, typename InputIterator>
void btrio::iiprintf(OutputIterator obegin, OutputIterator oend, InputIterator ibegin, InputIterator iend) {
    for (auto ocursor = obegin, icursor = ibegin; ocursor != oend && icursor != iend; ++ocursor, ++icursor) {
        *ocursor = *icursor;
    }
}

//~ ifprintf ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename InputIterator, typename T, typename... Ts>
void btrio::fiprintf(FILE* f, InputIterator begin, InputIterator end, T arg, Ts... args) {
    auto cursor = begin;

    while (cursor != end) {
        if (*cursor == '%') {
            ++cursor;
            if (cursor == end) {
                fput('%', f);
                break;
            }
            else if (*cursor == '_') {
                fput(arg, f);
                ++cursor;
                break;
            }
            else {
                fput('%', f);
                fput(*cursor, f);
            }
        }
        else {
            fput(*cursor, f);
        }
        ++cursor;
    }

    iprintf(cursor, end, args...);
}

template <typename InputIterator>
void btrio::fiprintf(FILE* f, InputIterator begin, InputIterator end) {
    for (auto cursor = begin; cursor != end; ++cursor) {
        fput(*cursor, f);
    }
}

//~ printf ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static constexpr auto format_fstring =
    "Format:\n"
    "    radix = %_\n"
    "    decimals = %_\n"
    "    padding zeros = %_\n"
    "    pad side = %_\n"
    "    minw = %_\n"
    "    maxw = %_\n"
    "    fill = '%_'\n"
    ;

template <typename F>
void btrio::print_format() {
    btrio::printf(format_fstring
                  , F::get_radix()
                  , F::get_decimals()
                  , F::get_padding_zeros()
                  , F::get_pad_side()
                  , F::get_minw()
                  , F::get_maxw()
                  , F::get_fill()
                  );
}

FORMAT_TEMPLATE
void btrio::print_format(btrio::static_format<FORMAT_ARGS>) {
    btrio::printf(format_fstring
                  , _radix
                  , _decimals
                  , _padding_zeros
                  , _pad_side
                  , _minw
                  , _maxw
                  , _fill
                  );
}

#endif // PRINT_HPP
