#
# Roberto Sosa Cano
# Copyright 2013
#

UNAME=$(shell uname)

# Mac OS alternate cmdline link options
ifeq ($(UNAME), Darwin)
LDFLAGS= -framework Carbon -framework OpenGL -framework GLUT
else
LDFLAGS= -lGL -lGLUT
endif

#
# Files to be compiled
#
LDFLAGS+= -L../lib -lengine

#
# Compilation flags
#
CXXFLAGS= -I../WindowManagers -I../Renderers -I../Object3D -g

all: square

square: square.o
	@g++ -o $@ $? $(LDFLAGS)

%.o: %.cpp
	@g++ $(CXXFLAGS) -c -o $@ $?

clean:
	@rm -f *.o
	@rm -f square
