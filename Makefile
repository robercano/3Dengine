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
VPATH=WindowManagers:Renderers:Object3D
FILES= main.cpp GLUTWindowManager.cpp OpenGLRenderer.cpp Object3D.cpp WindowManager.cpp

OBJDIR=obj
OBJECTS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(FILES))

#
# Compilation flags
#
CXXFLAGS= -IWindowManagers -IRenderers -IObject3D -g

#
# Project name
#
BINARY=engine

all: dirs $(BINARY)

dirs:
	mkdir -p $(OBJDIR)

$(BINARY): $(OBJECTS)
	g++ -o $@ $? $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	g++ $(CXXFLAGS) -c -o $@ $?

clean:
	rm $(OBJDIR)/*.o
	rmdir $(OBJDIR)
	rm $(BINARY)
