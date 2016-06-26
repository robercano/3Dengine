#
#Roberto Sosa Cano
#Copyright 2013
#

### Project name
PROJECT=engine

### System config
UNAME=$(shell uname)

CXX=g++
CC=gcc

#
#Files to be compiled
#
VPATH=core/src:core/opengl/src:core/procedural/src:utils/src

CORE_FILES=Game.cpp InputManager.cpp WindowManager.cpp TimeManager.cpp \
		   ModelLoaders.cpp \
		   Model3D.cpp \
		   TextConsole.cpp TrueTypeFont.cpp FreeTypeFont.cpp FontRenderer.cpp \
           Renderer.cpp NOAARenderTarget.cpp MSAARenderTarget.cpp SSAARenderTarget.cpp \
		   FXAARenderTarget.cpp FXAA2RenderTarget.cpp FBRenderTarget.cpp ToonRenderTarget.cpp \
		   ShadowMapRenderTarget.cpp \
		   Shader.cpp \
		   BlinnPhongShader.cpp ToonLightingShader.cpp NormalShadowMapShader.cpp \
		   FlyMotion.cpp FreeFlyMotion.cpp WalkingMotion.cpp \
		   Logging.cpp

UTILS_FILES=MathUtils.cpp ImageLoaders.c

OPENGL_FILES=GLFWKeyManager.cpp GLFWMouseManager.cpp GLFWWindowManager.cpp \
			 OpenGLModel3D.cpp \
			 OpenGLFontRenderer.cpp \
			 OpenGLRenderer.cpp OpenGLFilterRenderTarget.cpp \
             OpenGLMSAARenderTarget.cpp OpenGLSSAARenderTarget.cpp OpenGLFBRenderTarget.cpp \
			 OpenGLShadowMapRenderTarget.cpp \
             OpenGLShader.cpp OpenGLShaderMaterial.cpp \
			 OpenGLShaderPointLight.cpp OpenGLShaderSpotLight.cpp OpenGLShaderDirectLight.cpp \
			 OpenGLUniformBlock.cpp
#PROCEDURAL_FILES = Cube.cpp Icosahedron.cpp Plane.cpp Sphere.cpp
PROCEDURAL_FILES=Plane.cpp

FILES=$(CORE_FILES) $(OPENGL_FILES) $(PROCEDURAL_FILES) $(UTILS_FILES)

OBJDIR=obj
CPP_OBJECTS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(FILES))
OBJECTS=$(patsubst %.c,$(OBJDIR)/%.o,$(CPP_OBJECTS))

LIBDIR=lib
LIBNAME=lib$(PROJECT).$(SHAREDEXT)

DEMODIR=demos

#Mac OS alternate cmdline link options
ifeq ($(UNAME), Darwin)
LDFLAGS= -Llib -lengine -L/opt/X11/lib -lfreetype -lglew -lglfw3 -ljpeg -framework Cocoa -framework OpenGL -framework IOKit -fPIC
FLAGS=-I/opt/X11/include -I/opt/X11/include/freetype2/ -Wno-deprecated-register
SHAREDGEN= -dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup,-compatibility_version,1.0,-current_version,1.0,-install_name,$(LIBNAME)
SHAREDEXT=dylib
PREFIX=/usr/local/lib
else
LDFLAGS+= -Llib -lengine -lGL -lGLEW -lglfw3 -lpng -ljpeg -lfreetype -lX11 -lXrandr -lXinerama -lXi -lXxf86vm -lXcursor -ldl -pthread -fPIC
FLAGS=-I/usr/include -I/usr/include/freetype2
SHAREDGEN= -shared
SHAREDEXT=so
PREFIX=/usr/local/lib
endif

#
#Compilation flags
#
FLAGS+=-Werror -MMD -fPIC -Icore/inc -Icore/opengl/inc -Icore/procedural/inc -Iutils/inc -I3rdparty -g -DDEBUG_OPENGL_PIPELINE
CXXFLAGS=$(FLAGS) -std=c++11
CFLAGS=$(FLAGS) -std=c11

#
#Demos
#
DEMO_FILES=$(shell \ls demos/*.cpp)
DEMO_TARGETS=$(DEMO_FILES:.cpp=)

#
# Main rules
#
.PHONY: demos

all: engine $(DEMO_TARGETS)

engine: dirs $(LIBDIR)/$(LIBNAME)

demos/%: demos/%.cpp $(LIBDIR)/$(LIBNAME)
	@echo "- Compiling demo $@"
	@g++ $(CXXFLAGS) -o $@ $< $(LDFLAGS) 

dirs:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(LIBDIR)

$(LIBDIR)/$(LIBNAME): $(OBJECTS)
	@echo "- Generating $@...\c"
	@$(CXX) $(SHAREDGEN) -o $@ $(OBJECTS)
	@echo "done"

-include $(OBJECTS:.o=.d)

$(OBJDIR)/%.o: %.cpp
	@echo "- Compiling $<..."
	@if test -n "`which clang-format`"; then clang-format -i $<; fi
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJDIR)/%.o: %.c
	@echo "- Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@echo "- Cleaning project directories...\c"
	@rm -fr $(OBJDIR)
	@rm -fr $(LIBDIR)
	@echo "done"
