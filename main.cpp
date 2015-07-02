// Copyright (c) 2014-2015 Alexander Koch

/********************************************************************************/
/* CHANGELOG:                                                                   */
/* v1.0 - Voxel terrain code, generates chunk based on simplex noise            */
/* v1.1 - created GLFW Wrapper and Engine Mangager Class 'Context'              */
/* v1.2 - fixed shader class error (compilation of glsl)                        */
/* v1.3 - created property file parser                                          */
/* v1.4 - replaced readFile with new FileReader-class                           */
/* v1.5 - added framebuffer / post processing (SSAO, Color correction, Vignette)*/
/* v1.6 - added Font class for bitmap font rendering with font shaders,         */
/*      - added : FPSManager class, ObjLoader class                             */
/* v1.7 - huge update on ObjLoader (normal import and quad splitting)           */
/* v1.8 - removed old Vector3 class, using glm for now                          */
/* v1.9 - improved ObjLoader class, now rendering using vertexAttributes        */
/*      - added Sky class with Preetham sky model glsl shader                   */
/* v2.0 - diamond-square fractal terrain generation, world class                */
/* v2.1 - structured, World class, objloader update, finished FilmCamera(class) */
/* v2.2 - ShadowMapping implemented, texture class improved, terrain modified   */
/* v2.3 - Context class is now a wrapper, executes/runs a core instead of an    */
/*        engine, can create multiple cores by inheritance                      */
/* v2.4 - removed old render to texture code, replaced by PostComposer class    */
/* v2.5 - extended Mesh, ObjLoader classes, shadow optimizations                */
/* v2.6 - reworked the PostComposer code and shaders, implemented DoF shader    */
/* v2.7 - working on deferred rendering, implemented GeometryBuffer class,      */
/*      - updated Mesh class, working on mtl loading and material handling,     */
/*      - split view of normals, albedo, depth and final image                  */
/* v2.8 - improved / finished parseLines function in Tokenizer/FileReader class */
/*      - also basic glsl shader include directive, need to improve,            */
/*      - begin of android port, basic project setup with Core class,           */
/*      - found shadow map bug, texcoords are wrong when rendering from sun     */
/* v2.9 - implemented variance shadow mapping, added profiler class,            */
/*      - improved deferred renderer, cleaned shader code                       */
/* v3.0 - cleaned up code, inplemented new Console class, enabled HDR rendering */
/*      - updated sky and ground shaders, replaced MSAA with shader FXAA        */
/* v3.1 - improved Context usability, single two commands to load custom core,  */
/*      - also Console silent argument                                          */
/* v3.2 - great improvements, added base lua scripting, mouse fixes,            */
/*      - script reloading, realtime scripting, command line lua                */
/* v3.3 - replaced key struct by class ActionListener, handles all keys at once */
/*      - also implemented lua classes / scripts functionality                  */
/* v3.4 - added MeshRenderer class and Material class,                          */
/*      - 'replacing' the Mesh class, also integrated in lua, new makefile      */
/* v3.5 - added GenericType class (stores different types), added LuaMaterial,  */
/*      - and a MaterialLibrary class which stores materials, also UserMaterial */
/*      - json file material loading / uniform passing basics                   */
/* v3.6 - lua vec3, matrix, scene classes, new editor class, text editor        */
/* v3.7 - MaterialLibrary class, TextureLibrary class, lua scene extension      */
/*      - forward compatible context, materials in json format (.mat)           */
/* v3.8 - created new BulletPhysicsEngine class, implemented basic test         */
/*      - included bullet physics, TODO: implement in lua                       */
/*      - added command line arguments to context / core                        */
/* v3.9 - implemented basic lua bullet physics, sphere shape is default,        */
/*      - basic android port                                                    */
/* v4.0 - gui programming in progress, either node based or WYSIWYG, freetype   */
/*      - font rendering, new gui system, quaterion rotation in MeshRenderer    */
/*      - bullet rotation of rigidBodies using lua,                             */
/*      - Added lua demos: Spherical Harmonics, Bullet Physics and Shader test  */
/* v4.1 - lua camera port, designing user interface (Editor)                    */
/* v4.2 - replaced vec3 class by math library LuaMath, uses tables as arrays    */
/* v4.3 - rewritten materials, replaced by C++11 methods, std::shared_ptr<>()   */
/* v4.4 - node based editor, generates code based on connected nodes            */
/*      - implemented cube, sphere, plane meshes, values and numbers still to   */
/*      - develop, also have to implement a user input for custom values        */
/* v4.5 - optimized code and lua implementations, implemented variable collider */
/*      - physics can be tweaked in lua                                         */
/* v4.6 - removed old gui system code, using ide code instead, moving luaAPI    */
/*      - TODO: fix error on to many bullet bodies, crash                       */
/* v4.7 - problem was garbage collection of rigidbodies, not the source-code    */
/*      - updated physics system, testing python and added bulletcylinder       */
/*      - markdown file, licensing                                              */
/* v4.8 - removed python (backup), dependencies now fully integreated			*/
/*		- created makefile for dependencies, added README.md for info 			*/
/*      - TODO: Level files and Blender addon                                   */
/* v4.9 - FileReader update, introduced new Package class, now you can export   */
/*      - your project as a package file (archive file) TODO: compression       */
/* v5.0 - shadow map rendering re-implemented, now in scene class,              */
/*      - implementing gl class / namespace for opengl functions                */
/*      - debug drawing, editor development restarted using imgui               */
/*      - bullet rotation fix                                                   */
/* v5.1 - fixed lua crash, updated lua version to 5.3, created include function */
/*      - debug drawing function in lua + lua functions                         */
/*      - gl class reimplements depcrecated opengl function (new interpretation)*/
/*      - first person view control using lua and bullet (no fly)               */
/* v5.2 - editor improvement, now export of level files, python build script,   */
/*      - ITexture texture interface, Cubemap support in development,           */
/*      - small changes                                                         */
/* v5.3 - Cubemaps fully supported, SceneManager update, Frontend update,       */
/*      - network tests / development, Commons.h changes, Image based lighting  */
/* v5.4 - Package support!, *.pak system development now in a seperate project  */
/*      - basic package loading and running is working, TODO: fix *.pak issues  */
/* v5.5 - console logs classified, loading screen and removed dead code         */
/*      - TODO: Fix image based lighting                                        */
/*                                                                              */
/********************************************************************************/

/********************************************************************************/
/* TODO: (not sorted)                                                           */
/* - custom memory allocation, also leakcheck                                   */
/* - openal audio system                                                        */
/* - plug-in mechanism for lua scripted plug-ins								*/
/* - add Standard Pipeline and Fixed Pipeline rendering (standard or custom)    */
/* - Shading system / spot lights shadows                                       */
/* - support four lights, sort by nearest for shading							*/
/* - create standard shader for standard pipeline (custom shading language?)	*/
/* - particles, mesh animation, audio pipeline									*/
/* - add android export option  / webgl export option                           */
/* - lua render texture and camera class (both in camera)                       */
/* - add hdr eye adaption                                                       */
/* - global illumination                                                        */
/* - fully functional deferred rendering                                        */
/* - occlusion / frustum culling, faster obj loading, optimizations             */
/********************************************************************************/

#define ENGINE_TITLE "Engine demo v5.5"
#define EDITOR_TITLE "Frontend Editor v5.5"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <core/Core.h>
#include <core/Context.h>
#include <core/Exception.h>
#include <core/Console.h>

#include "Engine.h"
#include <gui/UIFrontend.h>

int main(int argc, char** argv)
{
    Console::log("///////// Game Engine /////////////////////");
    Console::log("Copyright (c), 2014-2015 Alexander Koch");
    std::vector<std::string> args(argv + 1, argv + argc + !argc);

#ifdef BUILD_ENGINE
    Engine engine;
    if(!Context::getInstance().load(ENGINE_TITLE, "engine.properties", false)) exit(1);
    Context::getInstance().run(&engine, args);
#endif

#ifdef BUILD_EDITOR
    UIFrontend editor;
    if(!Context::getInstance().load(EDITOR_TITLE, "editor.properties", false)) exit(1);
    Context::getInstance().run(&editor, args);
#endif

	return 0;
}
