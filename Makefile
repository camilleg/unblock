SRCS = main.cpp easybmp/EasyBMP.cpp 
SRCS_C = $(wildcard costella/*.c)

OBJS := ${SRCS:.cpp=.o}
OBJS += ${SRCS_C:.c=.o}
EXE = unblock

CFLAGS = -O3 -Wall -W
CXXFLAGS = $(CFLAGS) -Ieasybmp -Icostella

$(EXE): $(OBJS) Makefile
	g++ -o $@ $(OBJS) -lpng -lm

clean:
	rm -f $(EXE) $(OBJS)

test: $(EXE)
	./testcases.sh
