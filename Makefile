SRCS = main.cpp
SRCS_C = $(wildcard costella/*.c)

OBJS := ${SRCS:.cpp=.o}
OBJS += ${SRCS_C:.c=.o}
EXE = unblock

CFLAGS = -O3 -Wall -W
CXXFLAGS = $(CFLAGS) -Icostella

$(EXE): $(OBJS) Makefile
	g++ -o $@ $(OBJS) -lm -lpng

clean:
	rm -f $(EXE) $(OBJS)
