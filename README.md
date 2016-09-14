# Generic 3D Engine

## Introduction
This is a demo 3D engine set to showcase my 3D graphics and system programming skills. The main aim is to implement several known techniques and produce a series of demos to showcase this.

## Compilation

### Ubuntu 14.04 LTS

You need to install the following packages in order to compile the package:

    sudo apt-get install g++ cmake libglew-dev libjpeg-dev xorg-dev libfreetype6-dev clang-format

### Ubuntu 16.04 LTS

You need to install the following packages in order to compile the package:

    sudo apt-get install libglew-dev libglfw3-dev libjpeg-dev xorg-dev libfreetype6-dev clang-format

### GLFW3 library (only for Ubuntu 14.04 LTS)

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

The demos have been compiled with Visual Studio Community Edition. To compile the solution open the
solution file at the top of repository 3Dengine.sln. Build the whole solution. The demo executables
will be generated at the root of the repository. Then copy all the DLLs in 3rparty/windows/DLL/ to
the root of the repository and double click on any .exe to run the demo.

## Current set of features

* Class hierarchy designed to support different rendering APIs (OpenGL, DirectX)
* Cross-platform: Linux, Windows and OSX supported
* OpenGL renderer (based on GLFW and GLEW)
* Input manager (keyboard and mouse for now)
* Window manager (based on GLFW)
* Shader class to compile and apply shaders: vertex/geometry/fragment supported
* Render targets support
* Anti-aliasing render targets implemented with shaders: FXAA, FXAA2, SSAA and MSAA
    FXAA2 is my own implementation of FXAA based on Timothy Lottes paper
* HDR render target with Reinhard tone mapping
* Bloom effect shader
* Text rendering using TrueType fonts
* Walk, Fly and FreeFly cameras implemented with quaternions
* Goraud and flat illumination shaders (perhaps outdated, only goraud is used)
* Phong and Blinn-Phong reflection model shaders
* Dynamic lights support: point light, spot light and direct light
* Shadow map support for all dynamic lights
* Visual debug info: lights, normals, wireframe and bounding volumes
* Basic geometry culling using bounding volumes
* Basic light culling using bounding volumes (needs refinement)
* Common game loop with input, time and screen management
* OBJ format importer supporting geometry, textures and material specification
* Facility to save in-memory models to disk and load them back. This includes compression of all the data written to disk
* Scene management, now all elements are added to scene class and passed to renderer
* Procedural generation: Plane, bent plane, cylinder, torus, sphere, triangle and terrain (using Perlin noise)

# Demos
* Anti-aliasing methods showcase demo (FXAA, FXAA2, MSAA and SSAA)
* FXAA and FXAA2 side by side comparison
* Point lights demo
* Toon shader demo
* Shadow maps demos with direct light, point light and spot light
* Bounding box demo with OBB, AABB and Bounding Sphere
* Debug info demo showing wireframe, normals, bounding volumes and lights billboards
* Procedural generation demo using all supported models

## Screenshots

### Anti-aliasing

<a href="/demos/screenshots/AA-MSAA.png?raw=true"><img src="/demos/screenshots/AA-MSAA.png" alt="Antialiasing - MSAA" width="250px"/></a>
<a href="/demos/screenshots/AA-FXAA.png?raw=true"><img src="/demos/screenshots/AA-FXAA.png" alt="Antialiasing - FXAA" width="250px"/></a>
<a href="/demos/screenshots/AA-FXAA2.png?raw=true"><img src="/demos/screenshots/AA-FXAA2.png" alt="Antialiasing - FXAA2" width="250px"/></a>

### Bounding volumes

<a href="/demos/screenshots/BoundingBox.png?raw=true"><img src="/demos/screenshots/BoundingBox.png" alt="Bounding Volumes" width="250px"/></a>

### Debug information

<a href="/demos/screenshots/DebugInfo.png?raw=true"><img src="/demos/screenshots/DebugInfo.png" alt="Debug Information" width="250px"/></a>

### Procedural geometry

<a href="/demos/screenshots/Procedural.png?raw=true"><img src="/demos/screenshots/Procedural.png" alt="Procedural" width="250px"/></a>

### Toon effect 

<a href="/demos/screenshots/Toon.png?raw=true"><img src="/demos/screenshots/Toon.png" alt="Toon Effect" width="250px"/></a>

### Lights and Shadows 

<a href="/demos/screenshots/shadows-pointlight.png?raw=true"><img src="/demos/screenshots/shadows-pointlight.png" alt="Shadows - Point Light" width="250px"/></a>
<a href="/demos/screenshots/shadows-spotlight.png?raw=true"><img src="/demos/screenshots/shadows-spotlight.png" alt="Shadows - Spot Light" width="250px"/></a>
<a href="/demos/screenshots/shadows-moon.png?raw=true"><img src="/demos/screenshots/shadows-moon.png" alt="Shadows - Moon" width="250px"/></a>
<a href="/demos/screenshots/shadows-sun.png?raw=true"><img src="/demos/screenshots/shadows-sun.png" alt="Shadows - Sun" width="250px"/></a>
<a href="/demos/screenshots/shadows-sunset.png?raw=true"><img src="/demos/screenshots/shadows-sunset.png" alt="Shadows - Sunset" width="250px"/></a>

### HDR and Bloom effect

<a href="/demos/screenshots/HDR-off.png?raw=true"><img src="/demos/screenshots/HDR-off.png" alt="HDR - Off" width="250px"/></a>
<a href="/demos/screenshots/HDR-on.png?raw=true"><img src="/demos/screenshots/HDR-on.png" alt="HDR - On" width="250px"/></a>
<a href="/demos/screenshots/HDR-bloom.png?raw=true"><img src="/demos/screenshots/HDR-bloom.png" alt="HDR - Bloom" width="250px"/></a>
<a href="/demos/screenshots/HDR-bloom_exposurelow.png?raw=true"><img src="/demos/screenshots/HDR-bloom_exposurelow.png" alt="HDR - Bloom Low Exposure" width="250px"/></a>

## In the pipeline

* Support for vieports when rendering a scene
* Automatic shadow map frustum calculation for shadow map rendering
* Cascade shadow maps
* Ambient occlusion
* Mipmaps support
* Bump-mapping
* Portals implementation
* Extra anti-aliasing filters: CMAA, TXAA, MLAA
* Anisotropic filtering support and showcase
* Deferred rendering pipeline
* Implement a small game with the engine
