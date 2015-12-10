# 3D Game Engine

A Game Engine written in C++11 with an embedded Lua interpreter.

## Features:

- Bullet physics
- Lua for scripting and project files
- Wavefront OBJ mesh loading
- PNG and TGA image files loading
- Text rendering using Freetype

## Screenshots

![Screenshot0](./images/Screenshot0.png)

![Screenshot1](./images/Screenshot1.png)

## Scripting and projects

### Project structure

A project consists of a toplevel directory with the projects name and subfolders for the contents.

	project
	|
	|-- levels
	|-- meshes
	|-- scripts
	|	|-- main.lua
	|	`-- main.mat
	|
	|-- shaders
	|-- sounds
	`-- project.vproj

The project file (*.vproj) points to the script to be started at the beginning. An example project file
could look like this:
```lua
project = {
	name = "Game",
	script = "scripts/main.lua"
}
```

The name attribute is shown in the window title, the script is the initial script.

### Scripting

The main script has three different function for you to use. Below is a basic file shown.

```lua
scene = Scene()

function onInit()
	-- Your initialisation method
end

function onUpdate()
	-- The update method is called once per frame
end

function onDraw()
	-- Debug drawing
end
```

The scene variable lets you access the whole scene and modify it. For example you can add an object in the onInit method(),
save it in a global variable and move it in update by applying a force on it. Vectors are represented by a table e.g. {1,3,5} -
a three-dimensional vector with x=1, y=3, z=5.

If you want to declare your own class, you can use the method class().
The constructor is declared by an __init() method.
Example:

```lua
Sky = class()

function Sky:__init()
	self.color = 0xFF00FF
end

function Sky:render()
	--do some stuff here
end
```

Other script can be imported / included using the 'include'-function.
```lua
include("scripts/your/file/here.lua")
```

## Tech-Demos

The following tech-demos are available:

- game - A 3D Fist-person simulation
- dungeon - A pseudo-random dungeon structure
- sponza - The classic Sponza demo
- main - A atmospherical scattering demo
- physics - Use bullet physics to break a wall
- spherical - Tests for shaders
- a_star - The A*-Pathfinding algorithm

## Front-End

The frontend is created using [imgui](https://github.com/ocornut/imgui) and is still in development.

## For Developers

The project is compiled using a Makefile and uses MinGW.
To compile the project you have to download and install the dependencies for your system.
For the dependencies you need to have cmake installed.

### Dependencies

* [Bullet](http://bulletphysics.org/)
* [Freetype](http://www.freetype.org/)
* [GLEW](http://glew.sourceforge.net/)
* [GLFW](http://www.glfw.org/)
* [GLM](http://glm.g-truc.net/0.9.6/index.html)
* [lodepng](http://lodev.org/lodepng/)
* [Lua](http://www.lua.org/)
* [imgui](https://github.com/ocornut/imgui)

## License

Copyright (c) 2014-2015 Alexander Koch

Licensed under the Apache License, Version 2.0 (the "License");

You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
