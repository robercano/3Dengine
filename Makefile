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
LDFLAGS= -L/opt/X11/lib -lglew -lglfw -framework Cocoa -framework OpenGL -framework IOKit -fPIC
SHAREDGEN= -dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-install_name,
SHAREDEXT=dylib
PREFIX=/usr/local/lib
else
LDFLAGS= -lGL -lGLUT -fPIC
SHAREDGEN= -shared -Wl,-dynamiclib,
SHAREDEXT=so
PREFIX=/usr/local/lib
endif

#
# Files to be compiled
#
VPATH=WindowManagers:Renderers:Object3D:Game:Input:Camera
FILES=OpenGLRenderer.cpp WindowManager.cpp GLFWWindowManager.cpp GLFWKeyManager.cpp \
	  ColorCube.cpp GLFWMouseManager.cpp Game.cpp InputManager.cpp Camera.cpp

OBJDIR=obj
OBJECTS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(FILES))

LIBDIR=lib
LIBNAME=lib$(PROJECT).$(SHAREDEXT)

DEMODIR=Demos

#
# Compilation flags
#
CXXFLAGS= -fPIC -IWindowManagers -IRenderers -IObject3D -I Game -I Input -I Camera -I/opt/X11/include -I ./Libraries -g

#
# Main rules
#
.PHONY: demos

all: engine 

engine: dirs $(LIBDIR)/$(LIBNAME)

install: engine
	@echo "- Installing $(LIBNAME) into $(HOME)/$(LIBDIR)...\c"
	@cp $(LIBDIR)/$(LIBNAME) $(HOME)/$(LIBDIR)/
	@echo "done"

demos: engine install
	@echo "- Compiling project demos...\c"
	@make -C $(DEMODIR) 1>/dev/null
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
