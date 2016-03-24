# Generic 3D Engine

## Introduction
This is a demo 3D engine set to showcase my 3D graphics and system programming skills. The main aim is to implement several known techniques and produce a series of demos to showcase this.

## Compilation

### Ubuntu 14.04 LTS

You need to install the following packages in order to compile the package:

    sudo apt-get install g++ cmake libglew-dev libjpeg-dev libglfw-dev xorg-dev

Then you can compile the demos with:

	make clean demos

The binaries will be created in the demo directory.

## Current set of features

* Class hierarchy designed to support different rendering APIs (OpenGL, DirectX)
* Implementation aimed to be cross-platform with minimal porting effort (Linux and Mac supported, Windows on the way)
* OpenGL renderer (based on GLFW and GLEW)
* Input manager (keyboard and mouse for now)
* Window manager (based on GLFW)
* Shader class to compile and apply shaders
* OBJ format importer supporting geometry, textures and material specification
* Render targets support
* Anti-aliasing render targets implemented with shaders: FXAA, FXAA2, SSAA and MSAA
    FXAA2 is my own implementation of FXAA based on Timothy Lottes paper
* Text rendering using TrueType fonts
* Walk, Fly and FreeFly cameras implemented with quaternions
* Goraud and flat illumination shaders
* Phong and Blinn-Phong reflection model shaders
* Simple shader demo effect for render targets (waves)
* Dynamic lights support

# Current set of demos
* Anti-aliasing methods showcase demo (FXAA, FXAA2, MSAA and SSAA)
* FXAA and FXAA2 side by side comparison
* Dynamic lights demo
* Toon shader demo

## In the pipeline
* Windows support
* Shadow mapping
* Ambient occlusion
* Mipmaps support
* Refactoring for common game loop
* OBJ object importer tool: it converts from OBJ to custom binary format for the engine
* Bump-mapping
* Portals implementation
* More shaders effect like bloom effect
* Extra anti-aliasing filters: CMAA, TXAA, MLAA
* Anisotropic filtering support and showcase
* Deferred rendering pipeline
* Implement a small game with the engine

