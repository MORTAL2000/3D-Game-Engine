/*
 * Copyright 2015 Alexander Koch
 * File: Package.h
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

#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <cstdio>
using namespace std;

namespace Package
{
	/**
	 *	Create an archive based on a directory
	 *	@param directory Directory to compress
	 *	@return Error code
	 */
	int compress(const string& directory);

	/**
	 *	Create an archive based on a directory
	 *	@param directory Directory to compress
	 *	@param name Name of the archive
	 *	@return Error code
	 */
	int compress(const string& directory, const string& name);

	/**
	 *	Convert an archive back to individual files
	 *	@param archive Archive to decompress
	 *	@return Error code
	 */
	int decompress(const string& archive);

	/**
	 *	Read a file
	 *	@param archive Archive to read
	 *	@param filename File to read
	 *	@param buffer Content of the file
	 *	@param size Size of the buffer
	 *	@return Error code
	 */
	int read_file(const string& archive, const string& filename, unsigned char*& buffer, long& size);

	/**
	 *	Read a file
	 *	@param archive Archive to read
	 *	@param filename File to read
	 *	@param content Content of the file
	 *	@return Error code
	 */
	int read_file(const string& archive, const string& filename, string& content);

	/**
	 *	Get all filenames
	 *	@param archive Archive to read
	 *	@param files Filenames
	 *	@return Error code
	 */
	int get_files(const string& archive, vector<string>& files);

	/**
	 *	Convert error code of getError() to a string message
	 *	@code Code got from getError()
	 *	@return Error message
	 */
	string getErrorString(int code);
}

#endif
