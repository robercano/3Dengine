#
# Roberto Sosa Cano
# Copyright 2013
#
LDFLAGS+= -L../lib -lengine -lglew -lglfw -framework OpenGL
CXXFLAGS= -I../WindowManagers -I../Renderers -I../Object3D -I../Libraries -I../Game -I../Input -I../Camera -g

FILES=$(shell \ls *.cpp)
TARGETS=$(FILES:.cpp=)

all: $(TARGETS)
	@rm -rf *.dSYM

%: %.cpp
	@g++ $(CXXFLAGS) $(LDFLAGS) -o $@ $?

clean:
	@rm -f *.o
	@rm -f $(TARGETS)
