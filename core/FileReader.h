#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include "Tokenizer.h"
#include "Package.h"

namespace FileReader
{
	void setPackage(const std::string&);
	bool usesPackage();

	bool read(const std::string&, unsigned char*&, long*);
	bool read(const std::string&, std::string*);
	bool readLines(const std::string&, std::vector<std::string>*);
}

#endif
