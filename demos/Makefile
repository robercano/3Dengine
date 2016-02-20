#
# Roberto Sosa Cano
# Copyright 2013
#

# Check the environment
UNAME=$(shell uname)

# Mac OS alternate cmdline link options
ifeq ($(UNAME), Darwin)
LDFLAGS+= -L../lib -lengine -lglew -lglfw -framework OpenGL
else
LDFLAGS+= -L../lib -lengine -lGL -lGLEW -lglfw -fPIC
endif

CXXFLAGS= -I../core/inc -I../core/opengl/inc -I../core/prcedural/inc -I../3rdparty -g

FILES=$(shell \ls *.cpp)
TARGETS=$(FILES:.cpp=)

all: $(TARGETS)

%: %.cpp
	g++ $(CXXFLAGS) -o $@ $? $(LDFLAGS) 

clean:
	@rm -f *.o
	@rm -f $(TARGETS)
