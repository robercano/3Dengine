#
# Roberto Sosa Cano
# Copyright 2013
#

LDFLAGS+= -L../lib -lengine -lglew -lglfw -framework OpenGL
CXXFLAGS= -I../WindowManagers -I../Renderers -I../Object3D -g

all: square polylines geodetails polydetails opengl32

square: square.o
	@g++ -o $@ $? $(LDFLAGS)

polylines: polylines.o
	@g++ -o $@ $? $(LDFLAGS)

geodetails: geodetails.o
	@g++ -o $@ $? $(LDFLAGS)

opengl32: opengl32.o
	@g++ -o $@ $? $(LDFLAGS)

polydetails: polydetails.o
	@g++ -o $@ $? $(LDFLAGS)

%.o: %.cpp
	@g++ $(CXXFLAGS) -c -o $@ $?

clean:
	@rm -f *.o
	@rm -f square polylines geodetails polydetails opengl32
