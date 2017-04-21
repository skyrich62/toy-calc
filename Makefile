CXXFLAGS=-std=c++14 -g -I ../PEGTL/include $(TRACE)

all: main

OBJS = main.o

main.o: grammar.h main.cc

main: $(OBJS)
	$(CXX) -g $(OBJS) -o main

clean:
	rm -rf main.o main

trace:
	$(MAKE) TRACE=-DTRACE clean all
