#include "FileReader.h"

#include <iostream>

namespace FileReader
{
	std::string m_package_file;
	bool use_package = false;

	void setPackage(const std::string& package)
	{
		m_package_file = package;
		use_package = true;
	}

	bool usesPackage()
	{
		return use_package;
	}

	/**
	 *	Reads given file
	 *  @param filename File to read
	 *  @param buffer Readed characters
	 *  @param size Length
	 * 	@return Status
	 */
	bool read(const std::string& filename, unsigned char*& buffer, long* size)
	{
		if(!use_package)
		{
			FILE* file = fopen(filename.c_str(), "rb");
			if(!file) return false;
			fseek(file, 0, SEEK_END);
			*size = ftell(file);
			rewind(file);
			buffer = new unsigned char[*size+1];
			if(!buffer) return false;
			auto read = fread(buffer, 1, *size, file);
			if(read != *size) return false;
			buffer[*size] = '\0';
			fclose(file);
		}
		else
		{
			Package package;
			return package.decompress_file(m_package_file, filename, buffer, size);
		}
		return true;
	}

	/**
	 *	Reads given file
	 * 	@param filename File to read
	 * 	@param str String pointer
	 *  @return Status
	 */
	bool read(const std::string& filename, std::string* str)
	{
		unsigned char* buffer;
		long size;
		if(!read(filename, buffer, &size)) return false;
		std::stringstream ss;
		ss << buffer;
		delete[] buffer;
		*str = ss.str();
		return true;
	}

	/**
	 *	Reads given file
	 * 	@param filename File to read
	 * 	@param lines Vector pointer to store lines
	 *  @return Status
	 */
	bool readLines(const std::string& filename, std::vector<std::string>* lines)
	{
		std::string source;
		if(!read(filename, &source)) return false;
		*lines = Tokenizer::parseLines(source);
		return true;
	}
}
