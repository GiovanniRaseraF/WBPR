CXX=g++

PROG=maxflow-serial
SRCS=main.cpp graph.cpp mmio.cpp
OBJS=$(SRCS:.cpp=.o)

CXXFLAGS=-O3 -Wall -Wextra -std=c++11 -fopenmp -march=native -mtune=native
#CXXFLAGS+=-DDEBUG

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(PROG) $(OBJS)

test:
	./maxflow-serial -f test_graph.txt  -s 0 -t 6
	./maxflow-serial -f test2.txt  -s 0 -t 6

.PHONY: all clean
