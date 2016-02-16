#
# Roberto Sosa Cano
# Copyright 2013
#
# Mac OS alternate cmdline link options
ifeq ($(UNAME), Darwin)
LDFLAGS+= -L../lib -lengine -lglew -lglfw -framework OpenGL
else
LDFLAGS+= -L../lib -lengine -lGL -lGLEW -lglfw -fPIC
endif

CXXFLAGS= -I../Procedural -I../WindowManagers -I../Renderers -I../Object3D -I../Libraries -I../Game -I../Input -I../Camera -g

FILES=$(shell \ls *.cpp)
TARGETS=$(FILES:.cpp=)

all: $(TARGETS)

%: %.cpp
	g++ $(CXXFLAGS) -o $@ $? $(LDFLAGS) 

clean:
	@rm -f *.o
	@rm -f $(TARGETS)
