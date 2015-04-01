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
	/**
	 *	Specify to use an archive file / package
	 *	@param package Package to load
	 */
	void setPackage(const std::string& package);

	/**
	 *	Return if a package is currently used
	 *	@return Package is used
	 */
	bool usesPackage();

	/**
	 *	Reads given file
	 *  @param filename File to read
	 *  @param buffer Readed characters
	 *  @param size Length
	 * 	@return Status
	 */
	bool read(const std::string& filename, unsigned char*& buffer, long& size);

	/**
	 *	Reads given file
	 * 	@param filename File to read
	 * 	@param str String pointer
	 *  @return Status
	 */
	bool read(const std::string& filename, std::string& str);

	/**
	 *	Reads given file
	 * 	@param filename File to read
	 * 	@param lines Vector pointer to store lines
	 *  @return Status
	 */
	bool readLines(const std::string&, std::vector<std::string>&);
}

#endif
