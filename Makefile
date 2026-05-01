SRCS_CPP = main.cpp easybmp/EasyBMP.cpp
SRCS_C = $(wildcard costella/*.c)

OBJS := ${SRCS_CPP:.cpp=.o} ${SRCS_C:.c=.o}
EXE = unblock

CFLAGS = -O3 -Wall -W -Wextra
CXXFLAGS := $(CFLAGS) -std=c++20 -Ieasybmp -Icostella
#CXXFLAGS += -g -ggdb # for gdb
#CXXFLAGS += -g -mno-avx # for valgrind, to avoid "unrecognised instruction"

$(EXE): $(OBJS) Makefile
	g++ -o $@ $(OBJS) -lpng -lm

clean:
	rm -f $(EXE) $(OBJS) testcase/out*

test: $(EXE)
	./testcases.sh

.PHONY: all clean test
