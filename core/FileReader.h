/*
 * Copyright 2015 Alexander Koch
 * File: FileReader.h
 * Description: Useful methods for reading files.
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

#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include "Tokenizer.h"
#include "Package.h"
#include "FileIO.h"

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
