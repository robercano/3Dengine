#
# Roberto Sosa Cano
# Copyright 2013
#
LDFLAGS+= -L../lib -lengine -lglew -lglfw -framework OpenGL
CXXFLAGS= -I../WindowManagers -I../Renderers -I../Object3D -g

FILES=$(shell \ls *.cpp)
TARGETS=$(FILES:%.cpp=%.out)
BINARIES=$(FILES:%.cpp=%)

all: $(TARGETS)
	@for target in *.out; do mv $$target $${target%.*}; done

%.out: %.o
	@g++ $(LDFLAGS) -o $@ $?

%.o: %.cpp
	@g++ $(CXXFLAGS) -o $@ -c $?

clean:
	@rm -f *.o
	@rm -f $(BINARIES)
