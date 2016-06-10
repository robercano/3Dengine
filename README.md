# Generic 3D Engine

## Introduction
This is a demo 3D engine set to showcase my 3D graphics and system programming skills. The main aim is to implement several known techniques and produce a series of demos to showcase this.

## Compilation

### Ubuntu 14.04 LTS

You need to install the following packages in order to compile the package:

    sudo apt-get install g++ cmake libglew-dev libjpeg-dev xorg-dev

libglfw3 package is also needed but it does not come with Ubuntu 14.04 LTS. You can download GLFW 3.2 [here](https://github.com/glfw/glfw/releases/download/3.2/glfw-3.2.zip).

Unzip the package, go into the library directory and perform the following commands:

    cmake .
    make
    sudo make install
    sudo ldconfig

Then you can compile the demos with:

	make clean all

The binaries will be created in the demos directory. To run them just:

    ./demos/shadows
    ./demos/toon

And so on. It is important that the running directory is the root of the git repo as the
data assets are referenced from there.

### Windows

TODO

## Current set of features

* Class hierarchy designed to support different rendering APIs (OpenGL, DirectX)
* Cross-platform: Linux, Windows and OSX supported
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
* Goraud and flat illumination shaders (perhaps outdated, only goraud is used)
* Phong and Blinn-Phong reflection model shaders
* Simple shader demo effect for render targets (waves)
* Dynamic lights support: point light, spot light and direct light
* Shadow map support for all dynamic lights
* Common game loop with input, time and screen management

# Current set of demos
* Anti-aliasing methods showcase demo (FXAA, FXAA2, MSAA and SSAA)
* FXAA and FXAA2 side by side comparison
* Point lights demo
* Toon shader demo
* Shadow maps demos with direct light, point light and spot light

## In the pipeline
* Scene support so user can define a scene and rendering happens automatically from the camera view
* Automatic shadow map frustum calculation for shadow map rendering
* Cascade shadow maps
* Ambient occlusion
* Mipmaps support
* OBJ object importer tool: it converts from OBJ to custom binary format for the engine
* Bump-mapping
* Portals implementation
* More shaders effects like bloom effect
* Extra anti-aliasing filters: CMAA, TXAA, MLAA
* Anisotropic filtering support and showcase
* Deferred rendering pipeline
* Implement a small game with the engine
