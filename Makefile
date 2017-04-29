CXXFLAGS=-std=c++14 -g -I ../PEGTL/include $(TRACE)

all: main

OBJS = main.o nodes.o states.o pvisitor.o rvisitor.o

main.o: grammar.h states.h nodes.h main.cc

main: $(OBJS)
	$(CXX) -g $(OBJS) -o main

states.o: states.h nodes.h states.cc

nodes.o: nodes.h nodes.cc

pvisitor.o: pvisitor.cc pvisitor.h nodes.h

rvisitor.o: rvisitor.cc rvisitor.h nodes.h

clean:
	rm -rf $(OBJS) main

trace:
	$(MAKE) TRACE=-DTRACE clean all
