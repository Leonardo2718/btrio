#         Copyright Leonardo Banderali 2017 - 2017.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

CXX?=g++

BTRIO_INCL?= ../include

HEADERS= \
	$(BTRIO_INCL)/put.hpp \
	$(BTRIO_INCL)/btrio.hpp

TARGETS=\
		simple_test

simple_test: simple_test.cpp $(HEADERS)
	$(CXX) -std=c++14 -I$(BTRIO_INCL) simple_test.cpp -o $@

clean:
	rm -rf $(TARGETS)
