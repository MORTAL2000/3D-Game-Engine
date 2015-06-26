3D Game Engine (Project Voxel)
=============================================================================

Dieses Projekt stellt eine experimentelle Game Engine dar.
Das Projekt wird in C++ 11 entwickelt (C++0x) von Alexander Koch
Ziel ist es eine plattformübergreifende Engine zu entwickeln, die über eine oder
mehrere Skriptsprachen programmiert werden kann.
Derzeit ist dies die Lua-Skriptsprache.
Zusätzlich soll sie durch Plugins erweiterbar sein.

Features and Tools:

 * C++ 11
 * Bullet Physics
 * Lua für Skripts und Konfigurationsdateien
 * Wavefront OBJ
 * PNG und TGA Format
 * Freetype für Textdarstellung
 * ImGui Editor (in Entwicklung) WYSIWYG
 * [...]

Scripting / Lua-Skripts
-----------------------------------------------------------------------------

Ein Lua-Skript beinhaltet eine Initialisierungsmethode und kann weitere
Methoden optional definieren. So ist es möglich eine Update methode hinzuzufügen
oder eine Zeichnungsmethode. Folgendes Skript stellt das Standard-Skript dar.

    scene = Scene()

    -- Use this for initialization
    function onInit()

    end

    -- Update is called once per frame
    function onUpdate()

    end

    -- Draw debug gizoms here
    function onDraw()

    end


Tech-Demos
-----------------------------------------------------------------------------

Tech-Demos befinden sich im Verzeichnis examples.
Folgende Projekte sind vorhanden

 * dungeon
 * sponza
 * main
 * physics
 * spherical
 * a_star
 * game

Dungeon stellt eine zufällig generierte Struktur dar, main eine Graphik-Demo, physics eine Physik-Demo
mithilfe der Bullet-Physik, spherical eine Shader-Demo, a_star den A*-Algorithmus und Klassen in Lua, sponza eine Architekturvisualisierung.

Für Entwickler
-----------------------------------------------------------------------------

Um das Projekt zu kompilieren wird derzeitig ein Makefile verwendet (mingw32-make).
Öffnen sie eine Kommandozeile mit folgendem Inhalt im selben Ordner des Makefiles

    make / mingw32-make

Für den Editor der Game Engine (experimentell)

    make MODULE=Editor BUILD_TYPE=EDITOR

Oder verwenden Sie das Python-Skript

    python build.py <engine|editor>


### Dependencies ###

Die Game Engine ist abhängig von folgenden Bibliotheken.

* [Bullet](http://bulletphysics.org/)
* [Freetype](http://www.freetype.org/)
* [GLEW](http://glew.sourceforge.net/)
* [GLFW](http://www.glfw.org/)
* [GLM](http://glm.g-truc.net/0.9.6/index.html)
* [lodepng](http://lodev.org/lodepng/)
* [Lua](http://www.lua.org/)
* [imgui](https://github.com/ocornut/imgui)

Um diese für das System zu kompilieren:

	cd dependencies
	make

Dies installiert lua und glew für das System. GLM und lodepng müssen nicht kompiliert werden.
Alle anderen müssen zusätzlich noch erstellt werden (falls das Betriebssystem kein Windows-Derivat ist).
Es folgen die Befehle zum generieren.

**Bullet**

	cd build3
	premake4 gmake
	cd gmake
	make

**GLFW und Freetype**

	cmake . -G "MSYS Makefiles"
	make

Lizenz
-----------------------------------------------------------------------------

Der Lizenztype ist noch nicht absolut sicher. Folgend ist nur eine Kopie, falls die MIT Lizenz benutzt wird.

(The MIT License)

Copyright (c) 2014-2015 Alexander Koch

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


Links
-----------------------------------------------------------------------------
Links für Tools und nützliche Webseiten

 * http://choosealicense.com
 * https://github.com/pengwynn/flint
 * https://github.com/github/markup
 * https://github.com/stevedonovan/LDoc/blob/master/ldoc/markdown.lua
 * http://irrlicht.sourceforge.net/forum/viewtopic.php?f=4&t=14656

-----------------------------------------------------------------------------
