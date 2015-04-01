#include "FileReader.h"
#include "Console.h"

namespace FileReader
{
	std::string m_package_file;
	bool use_package = false;

	void setPackage(const std::string& package)
	{
		if(!package.empty())
		{
			m_package_file = package;
			use_package = true;
		}
	}

	bool usesPackage()
	{
		return use_package;
	}

	bool read(const std::string& filename, unsigned char*& buffer, long& size)
	{
		if(!use_package)
		{
			FILE* file = fopen(filename.c_str(), "rb");
			if(!file) return false;
			fseek(file, 0, SEEK_END);
			size = ftell(file);
			rewind(file);
			buffer = new unsigned char[size+1];
			if(!buffer) return false;
			auto read = fread(buffer, 1, size, file);
			if(read != size) return false;
			buffer[size] = '\0';
			fclose(file);
		}
		else
		{
			auto error = Package::read_file(m_package_file, filename, buffer, size);
			if(error)
			{
				Console::log("Package: %s", Package::getErrorString(error).c_str());
			}
			return error;
		}
		return true;
	}

	bool read(const std::string& filename, std::string& str)
	{
		unsigned char* buffer;
		long size;
		if(!read(filename, buffer, size)) return false;
		std::stringstream ss;
		ss << buffer;
		delete[] buffer;
		str = ss.str();
		return true;
	}

	bool readLines(const std::string& filename, std::vector<std::string>& lines)
	{
		std::string source;
		if(!read(filename, source)) return false;
		lines = Tokenizer::parseLines(source);
		return true;
	}
}
