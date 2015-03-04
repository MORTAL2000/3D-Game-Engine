

Readme info v1.2
--------------------------------------
--------------------------------------

-- finished implementing, readme no longer maintained

--
lua implementation to add(sorted by importance):
physics
camera (lookAt, setPosition, ...)
render to texture
--

--
C++:
search assets in project folder(.lua and .mat)
--



Readme info v1.1
--------------------------------------
--------------------------------------

--
lua implementations to add:
read properties from properties file
render to texture
camera (lookAt, setPosition, ...)
physics!!!
(lighting)
--

--
add cull face 'none' option in lua mesh
--

--
C++:
read all .json files in predefined materials folder
--

Readme info v1.0
--------------------------------------
--------------------------------------

brainstorms:
--
whoosh!!

lua project documentation / mission control tasks
--


description:
--
this part is highly experimental and will be implemented soon!
(Use Makefile hightlighting, looks best)
--


purpose:
--
put here the subclasses referenced in lua
make project based on lua scripts, c++ as backend
--


class description:
--
classes should inherit original C object / clone functions
classes are optimized to a lua class wrapper
--


examples:
--
LuaMesh.h
LuaApplication.h


LuaMesh contains Mesh rendering code and is registered by lua wrapper
LuaWorld contains all objects / lighting information, has add(Object*) and remove(Object*) functions
LuaApplication allows lua to get context information / add meshes to world etc.
--


implementation:
--
load main.lua at beginning, if not available report to user

call main.lua::init() at startup
call main.lua::update() as update loop
call main.lua::shutdown() as application clear / shutdown

a render method will not be implemented,
will run at background using c code, based on the added objects
(e.g. will store MeshRenderers at list)

also c function in lua that will signalize the shutdown of the engine
--


web:
--
(http://www.lua.org/notes/ltn005.html) - class example, create similar in lua class wrapper
--


ideas:
--
let user load custom shader files?
custom shader language ?

load meshes
application as context manager

divide program in two
---> the editor
---> the player (like blender's blenderplayer)

export works like this:
user has created his/her lua scripts

project export:
folder + lua files(compiled?) + player

player launches lua files at startup

create VirtualMachine class ?
--


editor:
--
thre to four gui components:
left hierachy + toolbar / launch button
middle game view
right object info / properties
down middle console

hierachy like this:

World
 | |
 | ----object0 (Mesh)
 |
 ----object1 (Empty)

if object selected display properties on left
also implement real time code reloading

editor has minimal / openframeworks like gui
--
