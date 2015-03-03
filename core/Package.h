#ifndef PACKAGE_H
#define PACKAGE_H

#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <algorithm>

#include "FileIO.h"

class Package
{
public:
	Package();
	~Package();

	void add_file(const std::string&);
	bool decompress_file(const std::string&, const std::string&, unsigned char*&, long*);

	bool compress(const std::string&);
	bool decompress(const std::string&, const std::string&);

	bool getContents(const std::string&, std::vector<std::string>*);
private:
	std::vector<std::string> m_files;

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
};

#endif
