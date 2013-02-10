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

CXXFLAGS=

#
# Project name
#
BINARY=engine

all: $(BINARY)

$(BINARY): main.o
	g++ -o $@ $? $(LDFLAGS)

%.o: %.cpp
	echo $(UNAME)
	g++ $(CXXFLAGS) -c -o $@ $?

clean:
	rm *.o
	rm $(BINARY)
