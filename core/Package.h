/** Created by Alexander Koch, Copyright (c) 2015 **/

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
	 *  @param directory Directory to compress
	 *	@return Error code
	 */
	int compress(const string& directory);
	
	/**
	 *	Create an archive based on a directory
	 *  @param directory Directory to compress
	 *	@param name Name of the archive
	 *	@return Error code
	 */
	int compress(const string& directory, const string& name);

	/**
	 *  Convert an archive back to individual files
	 *  @param archive Archive to decompress
	 *	@return Error code
	 */
	int decompress(const string& archive);
	
	/**
	 *	Read a file
	 *  @param archive Archive to read
	 * 	@param filename File to read
	 *	@param buffer Content of the file
	 *	@param size Size of the buffer
	 *	@return Error code
	 */
	int read_file(const string& archive, const string& filename, unsigned char*& buffer, long& size);

	/**
	 *	Read a file
	 *  @param archive Archive to read
	 * 	@param filename File to read
	 *	@param content Content of the file
	 *	@return Error code
	 */
	int read_file(const string& archive, const string& filename, string& content);

	/**
	 *	Get all filenames
	 * 	@param archive Archive to read
	 *  @param files Filenames
	 *	@return Error code
	 */
	int get_files(const string& archive, vector<string>& files);

	/**
	 *	Convert error code of getError() to a string message
	 * 	@code Code got from getError()
	 *	@return Error message
	 */
	string getErrorString(int code);
}

#endif
