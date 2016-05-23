/*
 * Copyright 2015 Alexander Koch
 * File: Project.h
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

#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <ctype.h>

#include <core/Console.h>
#include <core/FileReader.h>
#include <core/FileIO.h>
#include <core/StringUtils.h>
#include <core/Tokenizer.h>

class Project {
public:
	Project();
	~Project();

	/**
	 *	Creates a project
	 *  @param title Title of the project
	 */
	void create(const std::string& title);

	/**
	 *	Loads a given project file
	 *  @param filename File (*.vproj) to load
	 */
	bool load(const std::string& filename);

	/**
	 *	Writes / creates a project structure in given directory
	 *  @param path Path to create project in
	 */
	bool save(const std::string& path);
	bool save(const std::string& path, bool empty);

	/**
	 *	Returns the validity of the project
	 */
	bool isValid();

	std::string getTitle() {
		return m_title;
	}

	std::string getScript() {
		return m_script;
	}

	std::string getProjectFile() {
		return m_project_file;
	}

	std::string getPath() {
		return Tokenizer::getDirectory(m_project_file, false);
	}
private:
	std::string m_title;
	std::string m_script;
	std::string m_project_file;

	bool m_valid;
};

#endif
