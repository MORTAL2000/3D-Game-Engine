#include "Package.h"

Package::Package()
{

}

Package::~Package()
{

}

void Package::add_file(const std::string& filename)
{
	m_files.push_back(filename);
}

bool Package::decompress_file(const std::string& filename, const std::string& source, unsigned char*& buffer, long* size)
{
	FILE* file = fopen(filename.c_str(), "rb");
	if(!file) return false;

	package_header header;
	fread(&header, sizeof(package_header), 1, file);

	for(auto i = 0; i < header.file_count; i++)
	{
		package_file pak;
		fread(&pak, sizeof(package_file), 1, file);

		if(std::string(pak.filename) != source)
		{
			fseek(file, pak.size * sizeof(char), SEEK_CUR);
		}
		else
		{
			buffer = new unsigned char[pak.size+1];
			if(!buffer)
			{
				fclose(file);
				return "";
			}

			fread(buffer, sizeof(char), pak.size, file);
			fclose(file);
			buffer[pak.size] = '\0';
			*size = pak.size+1;
			return true;
		}
	}

	fclose(file);
	return false;
}

bool Package::compress(const std::string& filename)
{
	FILE* file = fopen(filename.c_str(), "wb");
	if(!file) return false;

	package_header header;
	strcpy(header.signature, "PAK");
	header.file_count = m_files.size();
	fwrite(&header, sizeof(header), 1, file);

	for(auto f : m_files)
	{
		FILE* fp = fopen(f.c_str(), "rb");
		if(!fp)	continue;
		fseek(fp, 0, SEEK_END);
	 	auto size = ftell(fp);
		rewind(fp);
		char* buffer = new char[size];
		if(!buffer)
		{
			fclose(fp);
			continue;
		}
		auto read = fread(buffer, 1, size, fp);
		if(read != size)
		{
			fclose(fp);
			continue;
		}
		fclose(fp);

		package_file pak;
		std::memset(&pak, 0, sizeof(package_file));
		strcpy(pak.filename, f.c_str());
		pak.size = size;
		fwrite(&pak, sizeof(pak), 1, file);
		fwrite(buffer, sizeof(char), size, file);
		delete[] buffer;
	}

	fclose(file);
	return true;
}

bool Package::decompress(const std::string& filename, const std::string& folder)
{
	if(folder != "./")
	{
		FileIO::createDirectory(folder);
	}

	FILE* file = fopen(filename.c_str(), "rb");
	if(!file) return false;

	package_header header;
	fread(&header, sizeof(package_header), 1, file);

	for(auto i = 0; i < header.file_count; i++)
	{
		package_file pak;
		fread(&pak, sizeof(package_file), 1, file);

		char* buffer = new char[pak.size];
		if(!buffer)
		{
			fclose(file);
			return false;
		}

		fread(buffer, sizeof(char), pak.size, file);

		std::string temp(pak.filename);
		std::replace(temp.begin(), temp.end(), '/', '.');
		std::string fn = folder + "/" + temp;
		FILE* fp = fopen(fn.c_str(), "wb");
		if(!fp)
		{
			fclose(file);
			return false;
		}

		fwrite(buffer, sizeof(char), pak.size, fp);
		fclose(fp);

		delete[] buffer;
	}

	fclose(file);
}

bool Package::getContents(const std::string& filename, std::vector<std::string>* files)
{
	FILE* file = fopen(filename.c_str(), "rb");
	if(!file) return false;

	package_header header;
	fread(&header, sizeof(package_header), 1, file);

	for(auto i = 0; i < header.file_count; i++)
	{
		package_file pak;
		fread(&pak, sizeof(package_file), 1, file);
		fseek(file, pak.size * sizeof(char), SEEK_CUR);
		(*files).push_back(std::string(pak.filename));
	}

	fclose(file);
	return true;
}
