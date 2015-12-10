# Building (Libraries)

## Bullet

	$ cmake . -G "MinGW Makefiles"
	$ mingw32-make install

Fails building the demos. The rest should work fine.
Move the libraries out of the folders in src.

We just need BulletDynamics, BulletCommon and LinearMath. You can build them individualy.

## Bullet (32-bit issue - old mingw version)

- Modified generated makefile not to build the demos (demos were causing errors)
- Modified btQuickprof.cpp (replaced getTickCount64 by get_tick_count())

**unsigned long long**
get_tick_count(**void**)
{
if WINVER >= 0x0600
	return GetTickCount64();
else
	return GetTickCount();
endif
}

## Freetype

	$ cmake . -G "MSYS Makefiles"
	$ mingw32-make

## GLEW

- Download binaries online (http://glew.sourceforge.net/)

## GLFW3

	$ cmake . -G "MinGW Makefiles"
	$ cd src
	$ mingw32-make

## GLM

- Nothing to build

## imgui

- Nothing to build

## lodepng

- Nothing to build

## Lua

	$ mingw32-make mingw

## OpenAL-soft

	$ cd build
	$ cmake .. -G "MinGW Makefiles"
	$ mingw32-make

Author
---
Alexander Koch 2015
