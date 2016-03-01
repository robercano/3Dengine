# Generic 3D Engine

## Introduction
This is a demo 3D engine set to showcase my 3D graphics and system programming skills. The main aim is to implement several known techniques and produce a series of demos to showcase this.

## Current set of features

* OpenGL renderer (based on GLFW and GLEW)
* Input manager (keyboard and mouse for now)
* Window manager (based on GLFW)
* Shader class to compile and apply shaders
* Generic interface for all API dependant classes (like OpenGL renderer) that
  allows for a different API to be used (like DirectX)
* Multi-platform compilation (Linux and Mac for now)
* OBJ format importer
* Render targets
* Anti-alising render targets (SSAA and MSAA for now)
* Text rendering using TrueType fonts
* Fly camera
* Goraud and flat illumination shaders
* Phong and Blinn-Phong reflection model shaders
* Simple shader demo effect for render targets (waves)

## In the pipeline

* Anti-aliasing methods showcase demo
* Support for OBJ material and texture specification. Include geometry groups support.
* Implement FXAA
* Add proper light specification support (parametrize shader)
* Use quaternions for camera
* Ambient occlusion
