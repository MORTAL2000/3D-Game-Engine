/** Created by Alexander Koch, Copyright (c) 2015 **/

#include "Package.h"

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef __WIN32
	#include <windows.h>
	#ifdef __CYGWIN__
		#define mkdir(file) mkdir(file, 0777)
	#else
		#define mkdir _mkdir
		#define rmdir _rmdir
	#endif

	#define stat _stat
	#define S_IFDIR _S_IFDIR
#else
	#define mkdir(file) mkdir(file, 0777)
#endif

#define PAK_ERROR_NONE 0
#define PAK_ERROR_FILE 1
#define PAK_ERROR_ALLOC 2
#define PAK_ERROR_READ 3
#define PAK_ERROR_CONTENT 4
#define PAK_ERROR_ARCHIVE 5

namespace Package
{
	#pragma pack(1)
	struct package_header
	{
		char signature[3];
		long file_count;
	};

	struct package_file
	{
		char filename[256];
		long size;
	};
	#pragma pack()

	bool inDir(const string& path, string& dir)
	{
		unsigned found = path.find_last_of("\\/");
		if(found == (unsigned)-1) return false;
		dir = path.substr(0, found);
		return true;
	}

	bool is_dir(const char* path)
	{
    	struct stat buf;
    	stat(path, &buf);
    	return S_ISDIR(buf.st_mode);
	}

	void mkdirRecursive(const char* dir)
	{
		string subDir;
		if(inDir(dir, subDir))
		{
			mkdir(subDir.c_str());
		}
		mkdir(dir);
	}

	vector<string> getFilesInDirectory(const string& path)
	{
		vector<string> files;
		struct dirent* ent;
		auto dir = opendir(path.c_str());
		if(dir)
		{
			while((ent = readdir(dir)))
			{
				string curr(ent->d_name);
				if(curr == "." || curr == "..") continue;

				curr = path + "/" + ent->d_name;
				if(is_dir(curr.c_str()))
				{
				 	auto sub_files = getFilesInDirectory(curr);
					for(unsigned i = 0; i < sub_files.size(); i++) files.push_back(sub_files[i]);
				}
				else files.push_back(curr);
			}
		}
		closedir(dir);
		return files;
	}
	
	/** Begin methods **/
	
	int compress(const string& directory)
	{
		return compress(directory, directory + ".pak");
	}

	int compress(const string& directory, const string& name)
	{
		if(!is_dir(directory.c_str()))
		{
			return PAK_ERROR_FILE;
		}

		FILE* file = fopen(name.c_str(), "wb");
		if(!file)
		{
			return PAK_ERROR_FILE;
		}

	 	auto files = getFilesInDirectory(directory);
		package_header header;
		strcpy(header.signature, "PAK");
		header.file_count = files.size();
		fwrite(&header, sizeof(header), 1, file);

		for(auto f : files)
		{
			FILE* fp = fopen(f.c_str(), "rb");
			if(!fp)
			{
				fclose(file);
				return PAK_ERROR_FILE;
			}
			fseek(fp, 0, SEEK_END);
		 	auto size = ftell(fp);
			rewind(fp);
			char* buffer = new char[size];
			if(!buffer)
			{
				fclose(fp);
				fclose(file);
				return PAK_ERROR_ALLOC;
			}
			auto read = fread(buffer, 1, size, fp);
			if(read != size)
			{
				fclose(fp);
				fclose(file);
				delete[] buffer;
				return PAK_ERROR_READ;
			}
			
			fclose(fp);
			package_file pak;
			memset(&pak, 0, sizeof(package_file));
			strcpy(pak.filename, f.c_str());
			pak.size = size;
			fwrite(&pak, sizeof(pak), 1, file);
			fwrite(buffer, sizeof(char), size, file);
			delete[] buffer;
		}

		fclose(file);
		return PAK_ERROR_NONE;
	}

	int decompress(const string& archive)
	{
		FILE* file = fopen(archive.c_str(), "rb");
		if(!file)
		{
			return PAK_ERROR_FILE;
		}

		package_header header;
		fread(&header, sizeof(package_header), 1, file);

		char sig[4];
		strcpy(sig, header.signature);
		sig[3] = '\0';
		if(string(sig) != "PAK")
		{
			fclose(file);
			return PAK_ERROR_ARCHIVE;
		}

		for(auto i = 0; i < header.file_count; i++)
		{
			package_file pak;
			fread(&pak, sizeof(package_file), 1, file);

			char* buffer = new char[pak.size];
			if(!buffer)
			{
				fclose(file);
				return PAK_ERROR_ALLOC;
			}

			fread(buffer, sizeof(char), pak.size, file);

			string dir;
			if(inDir(pak.filename, dir))
			{
				mkdirRecursive(dir.c_str());
			}

			FILE* fp = fopen(pak.filename, "wb");
			if(!fp)
			{
				fclose(file);
				delete[] buffer;
				return PAK_ERROR_FILE;
			}
			fwrite(buffer, sizeof(char), pak.size, fp);
			fclose(fp);
			delete[] buffer;
		}

		fclose(file);
		return PAK_ERROR_NONE;
	}
	
	int read_file(const string& archive, const string& filename, unsigned char*& buffer, long& size)
	{
		FILE* file = fopen(archive.c_str(), "rb");
		if(!file)
		{
			return PAK_ERROR_FILE;
		}

		package_header header;
		fread(&header, sizeof(package_header), 1, file);

		for(auto i = 0; i < header.file_count; i++)
		{
			package_file pak;
			fread(&pak, sizeof(package_file), 1, file);

			if(string(pak.filename) != filename)
			{
				fseek(file, pak.size * sizeof(char), SEEK_CUR);
			}
			else
			{
				buffer = 0;
				buffer = new unsigned char[pak.size+1];
				if(!buffer)
				{
					fclose(file);
					return PAK_ERROR_ALLOC;
				}

				fread(buffer, sizeof(char), pak.size, file);
				buffer[pak.size] = '\0';
				size = pak.size;
				fclose(file);
				return PAK_ERROR_NONE;
			}
		}

		fclose(file);
		return PAK_ERROR_CONTENT;
	}
	
	int read_file(const string& archive, const string& filename, string& content)
	{
		unsigned char* buffer;
		long size;
		int error = read_file(archive, filename, buffer, size);

		stringstream ss;
		ss << buffer;
		delete[] buffer;
		content = ss.str();
		
		return error;
	}

	int get_files(const string& archive, vector<string>& files)
	{
		files.clear();
		FILE* file = fopen(archive.c_str(), "rb");
		if(!file)
		{
			return PAK_ERROR_FILE;
		}

		package_header header;
		fread(&header, sizeof(package_header), 1, file);

		for(auto i = 0; i < header.file_count; i++)
		{
			package_file pak;
			fread(&pak, sizeof(package_file), 1, file);
			files.push_back(pak.filename);
			fseek(file, pak.size * sizeof(char), SEEK_CUR);
		}

		return PAK_ERROR_NONE;
	}

	string getErrorString(int code)
	{
		switch(code)
		{
			case PAK_ERROR_NONE:
				return "No error";
			case PAK_ERROR_FILE:
				return "File / folder could not be found or created";
			case PAK_ERROR_ALLOC:
				return "Could not allocate memory";
			case PAK_ERROR_READ:
				return "Could not read file";
			case PAK_ERROR_CONTENT:
				return "Requested content not found";
			case PAK_ERROR_ARCHIVE:
				return "Requested file is not an archive file";
		}
		return "Unknown error";
	}

}
