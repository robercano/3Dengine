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

#
# Files to be compiled
#
VPATH=core/src:core/opengl/src:core/procedural/src

CORE_FILES=Camera.cpp Game.cpp InputManager.cpp OBJFormat.cpp Renderer.cpp WindowManager.cpp
OPENGL_FILES=GLFWKeyManager.cpp GLFWMouseManager.cpp GLFWWindowManager.cpp OpenGLRenderTarget.cpp \
             OpenGLRenderer.cpp OpenGLShader.cpp OpenGLObject3D.cpp
#PROCEDURAL_FILES=Cube.cpp Icosahedron.cpp Plane.cpp Sphere.cpp

FILES=$(CORE_FILES) $(OPENGL_FILES) $(PROCEDURAL_FILES)

OBJDIR=obj
OBJECTS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(FILES))

LIBDIR=lib
LIBNAME=lib$(PROJECT).$(SHAREDEXT)

DEMODIR=demos

# Mac OS alternate cmdline link options
ifeq ($(UNAME), Darwin)
LDFLAGS= -L/opt/X11/lib -lglew -lglfw -framework Cocoa -framework OpenGL -framework IOKit -fPIC
SHAREDGEN= -dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-install_name,$(LIBNAME)
SHAREDEXT=dylib
PREFIX=/usr/local/lib
else
LDFLAGS= -lGL -lGLEW -lglfw -fPIC
SHAREDGEN= -shared
SHAREDEXT=so
PREFIX=/usr/local/lib
endif

#
# Compilation flags
#
CXXFLAGS= -MMD -fPIC -Icore/inc -Icore/opengl/inc -Icore/procedural/inc -I/opt/X11/include -I3rdparty -g -DDEBUG_OPENGL_PIPELINE

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
	@make -C $(DEMODIR) >/dev/null 2>&1
	@echo "done"

dirs:
	@echo "- Creating project dirs...\c"
	@mkdir -p $(OBJDIR)
	@mkdir -p $(LIBDIR)
	@mkdir -p $(HOME)/$(LIBDIR)
	@echo "done"

$(LIBDIR)/$(LIBNAME): $(OBJECTS)
	@echo "- Generating $@...\c"
	@$(CXX) $(SHAREDGEN) -o $@ $(OBJECTS) $(LDFLAGS)
	@echo "done"

-include $(OBJECTS:.o=.d)

$(OBJDIR)/%.o: %.cpp
	@echo "- Compiling $<...\c"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<
	@echo "done"

clean:
	@echo "- Cleaning project directories...\c"
	@rm -fr $(OBJDIR)
	@rm -fr $(LIBDIR)
	@make -C $(DEMODIR) clean >/dev/null 2>&1
	@echo "done"
