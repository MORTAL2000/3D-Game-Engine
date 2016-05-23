/*
 * Copyright 2015 Alexander Koch
 * File: Project.cpp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Project.h"
#include <lua/LuaScript.h>

Project::Project() : m_title(""), m_script(""), m_valid(false) {}

Project::~Project() {}

void Project::create(const std::string& title) {
	m_title = title;
}

bool Project::load(const std::string& filename) {
	if(Tokenizer::getFileExtension(filename) != "vproj") {
		return false;
	}

	LuaScript script(filename);
	m_title = script.get<std::string>("project.name");
	m_script = script.get<std::string>("project.script");
	script.close();
	m_project_file = filename;
	return (m_valid = true);
}

bool Project::save(const std::string& path) {
	save(path, false);
}

bool Project::save(const std::string& path, bool empty) {
	std::string vproj = StringUtils::simplify(m_title);
	std::string folder = path + "/" + vproj;

	// examples/test
	FileIO::createDirectory(folder);
	FileIO::createDirectory(folder+"/maps");
	FileIO::createDirectory(folder+"/meshes");
	FileIO::createDirectory(folder+"/scripts");
	FileIO::createDirectory(folder+"/shaders");
	FileIO::createDirectory(folder+"/sounds");
	if(!empty) m_script = "scripts/main.lua";

	// create project (*.vproj) file
	std::string proj_file = folder + '/' + vproj + ".vproj";
	FILE* file = fopen(proj_file.c_str(), "wb");
	if(!file) return false;
	fprintf(file, "project = {\n");
	fprintf(file, "\tname = \"%s\",\n", m_title.c_str());
	fprintf(file, "\tscript = \"%s\"\n", m_script.c_str());
	fprintf(file, "}\n");
	fclose(file);

	if(!empty) {
		// create script (*.lua) file
		std::string script_file = folder + "/scripts/main.lua";
		file = fopen(script_file.c_str(), "wb");
		if(!file) return false;
		std::string content =
		"scene = Scene()\n\n"
		"-- Use this for initialization\n"
		"function onInit()\n\n"
		"end\n\n"
		"-- Update is called once per frame\n"
		"function onUpdate()\n\n"
		"end\n\n"
		"-- Draw debug gizoms here\n"
		"function onDraw()\n\n"
		"end\n";
		fprintf(file, "%s", content.c_str());
		fclose(file);

		//create material (*.mat) file
		std::string mat_file = folder + "/scripts/main.mat";
		file = fopen(mat_file.c_str(), "wb");
		if(!file) return false;
		content =
		"-- Material file\n"
		"textures = {}\n"
		"cubemaps = {}\n"
		"materials = {}\n";
		fprintf(file, content.c_str());
		fclose(file);
	}

	Console::log("Created project %s", m_title.c_str());
	return true;
}

bool Project::isValid() {
	return m_valid;
}
