#ifndef PROJECT_H
#define PROJECT_H

#include <string>
#include <ctype.h>

#include <core/Console.h>
#include <core/FileReader.h>
#include <core/FileIO.h>
#include <core/StringUtils.h>
#include <core/Tokenizer.h>
#include <core/Package.h>

class Project
{
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
	 * 	Exports the game as packaged runtime
	 */
	void exportAsRuntime();

	/**
	 *	Returns the validity of the project
	 */
	bool isValid();

	std::string getTitle()
	{
		return m_title;
	}

	std::string getScript()
	{
		return m_script;
	}

	std::string getProjectFile()
	{
		return m_project_file;
	}

	std::string getPath()
	{
		return Tokenizer::getDirectory(m_project_file, false);
	}
private:
	std::string m_title;
	std::string m_script;
	std::string m_project_file;

	bool m_valid;
};

#endif
