UNAME_S := $(shell sh -c 'uname -s 2>/dev/null')
SRCS_CPP = main.cpp easybmp/EasyBMP.cpp
SRCS_C = $(wildcard costella/*.c)

OBJS := ${SRCS_CPP:.cpp=.o} ${SRCS_C:.c=.o}
EXE = unblock

CFLAGS = -O3 -Wall -W -Wextra
CXXFLAGS := $(CFLAGS) -std=c++20 -Ieasybmp -Icostella
ifeq ($(UNAME_S),Darwin)
  CXXFLAGS += -I/opt/homebrew/include
  LDLIBS += -L/opt/homebrew/lib
endif
#CXXFLAGS += -g -ggdb # for gdb
#CXXFLAGS += -g -mno-avx # for valgrind, to avoid "unrecognised instruction"

$(EXE): $(OBJS) Makefile
	g++ -o $@ $(OBJS) $(LDLIBS) -lpng -lm

clean:
	rm -f $(EXE) $(OBJS) testcase/out*

test: $(EXE)
ifeq ($(UNAME_S),Darwin)
	@echo "On macOS this test will fail, because a very few pixels differ."
endif
	./testcases.sh

.PHONY: all clean test
