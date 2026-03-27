<img width="550" height="350" alt="tudo" src="tudo.png" />   

- [About](#about)
- [Features](#features)
- [Building](#building)
- [Solution Structure](#solution-structure)
- [Engine Design](#engine-design)
- [SHDC](#shdc)
- [M3DC](#m3dc)
- [Third-party libraries used](#third-party-libraries-used)

## 
<img width="1280" height="750" alt="screenshot" src="https://github.com/user-attachments/assets/4e50a2ef-691c-4ba0-85ea-949875abaea0" />
  
## About
Tudo is not a full-fledged game engine in the traditional sense,
but rather a minimal engine skeleton that supplies only the
fundamental components needed for game development.

## Features
* <b>Core:</b>
  * simple smart pointer
  * ini file reading/writing
  * alarm driven event system
  * fixed-timestep clock
  * program interface
* <b>Draw:</b>
  * shader loading & compiling
  * framebuffer managing
  * pipeline interface
  * renderer interface
  * advanced 2d-sprite renderer
	  * draw color quad
	  * draw sprite
	  * draw sprite atlas
	  * draw sprite instanced
	  * draw sprite atlas instanced
	  * draw sprite font text
	  * draw sprite sheet animation
  * 3d model renderer interface
	  * basic 3d billboard renderer
	  * basic 3d color mesh renderer
	  * basic 3d unlit mesh renderer
* <b>Math:</b>
  * vector 2d
  * vector 3d
  * vector 4d
  * matrix 4x4
  * quaternion
  * transformation
  	* translate
  	* scale
  	* rotate
  	* quat from axis angle
  	* quat from euler angles
  	* quat add euler rotation
  * projection
  	* look at lh
  	* project ortho lh
  	* project persp lh
  * screen location to world location
  * 2d collision
  	* aabb
  	* circle
  * 3d collision
  	* aabb
  	* sphere
* <b>Networking:</b>
  * tcp server/client
  * udp server/client
* <b>OS:</b>
  * multiple windows
  * keyboard, mouse and gamepad input
  * audio (via soloud)
* <b>Resources:</b>
  * simple binary archive
  * 3d model loading (custom gpu-friendly format)
  * automatic texture mipmap generation
 
## Building

Supported Platforms:
* Windows 10 	x64
* macOS 		arm64

Requirements:
* python3
* cmake
  
After cloning the repository, navigate to the Project directory
and generate the solution using CMake. This step requires
Python3 to be installed on your system.

Next, open the generated solution in either VS2022 or Xcode and
build the project.

If you encounter a runtime error on startup, it is likely because
the shaders have not yet been compiled. In that case, you must
first compile them using the offline tool SHDC.

On macOS, ensure that the Data folder (including all its contents)
is copied into the application’s Resources directory.

## Solution Structure

The solution is split into two projects: Tudo and Game.

Tudo is a static library, while Game is the main project where the
game is built on top of it.

## Engine Design

The engine is designed to be highly modular. You are responsible
for implementing your own scene management and graphics pipeline.
Create a DrawPipeline interface and use the existing renderer
classes (or implement your own) to draw primitives.

The overall architecture is up to you. Tudo only provides the
essential tools and interfaces.

## SHDC

This is a small utility tool for automatically locating and
compiling shaders. It supports compilation for D3D and SPIR-V on
Windows, and Metal on macOS.

```
Valid Arguments: 	<shader path> 	<dev path>
example use:		Data/Shaders	Data/Development/Shaders
```

## M3DC

This is a small utility tool that converts 3D files from modeling
software into a format supported by Tudo.

```
Valid Arguments: 	<input> 	<output>
example use:		box.obj		box.m3d
```
 
## Third-party libraries used:
* [stb_image](https://github.com/nothings/stb)
* [SoLoud](https://solhsa.com/soloud/)
* [SDL3](https://github.com/libsdl-org/SDL)
* [bgfx](https://github.com/bkaradzic/bgfx)
* [assimp](https://github.com/assimp/assimp)
