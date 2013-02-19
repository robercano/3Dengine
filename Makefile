#
# Roberto Sosa Cano
# Copyright 2013
#

### Project name
PROJECT=engine

### System config

UNAME=$(shell uname)

CXX=g++
CC=gcc

# Mac OS alternate cmdline link options
ifeq ($(UNAME), Darwin)
LDFLAGS= -framework Carbon -framework OpenGL -framework GLUT
SHAREDGEN= -dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-install_name,
SHAREDEXT=dylib
PREFIX=/usr/local/lib
else
LDFLAGS= -lGL -lGLUT
SHAREDGEN= -shared -Wl,-dynamiclib,
SHAREDEXT=so
PREFIX=/usr/local/lib
endif

#
# Files to be compiled
#
VPATH=WindowManagers:Renderers:Object3D
FILES=GLUTWindowManager.cpp OpenGLRenderer.cpp Object3D.cpp WindowManager.cpp

OBJDIR=obj
OBJECTS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(FILES))

LIBDIR=lib
LIBNAME=lib$(PROJECT).$(SHAREDEXT)

DEMODIR=Demos

#
# Compilation flags
#
CXXFLAGS= -fpic -IWindowManagers -IRenderers -IObject3D -g

#
# Main rules
#
.PHONY: demos

all: dirs $(LIBDIR)/$(LIBNAME)

install: all
	@echo "- Installing $(LIBNAME) into $(HOME)/$(LIBDIR)...\c"
	@cp $(LIBDIR)/$(LIBNAME) $(HOME)/$(LIBDIR)/
	@echo "done"

demos: all
	@echo "- Compiling project demos...\c"
	@make -C $(DEMODIR)
	@echo "done"

dirs:
	@echo "- Creating project dirs...\c"
	@mkdir -p $(OBJDIR)
	@mkdir -p $(LIBDIR)
	@mkdir -p $(HOME)/$(LIBDIR)
	@echo "done"

$(LIBDIR)/$(LIBNAME): $(OBJECTS)
	@echo "- Generating $@...\c"
	@$(CXX) $(SHAREDGEN)$(LIBNAME) -o $@ $? $(LDFLAGS)
	@echo "done"

$(OBJDIR)/%.o: %.cpp
	@echo "- Compiling $?...\c"
	@$(CXX) $(CXXFLAGS) -c -o $@ $?
	@echo "done"

clean:
	@echo "- Cleaning project directories...\c"
	@rm -fr $(OBJDIR)
	@rm -fr $(LIBDIR)
	@make -C $(DEMODIR) clean
	@echo "done"
