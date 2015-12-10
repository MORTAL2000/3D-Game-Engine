#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <sstream>
#include <string>
#include <vector>
#include <ctype.h>
using vector;
using string;

namespace Tokenizer
{
	vector<string> tokenize(string, char);
	string getFileExtension(const string&);
	string getDirectory(const string&);
	string getDirectory(const string&, bool);
	string removePath(const string&);
	string removeWhitespace(const string&);
	vector<string> parseLines(const string&);
	string removeCharacter(const string&, char);
	vector<std::size_t> findAll(const string&, const string&);
	vector<std::size_t> findAllDigits(const string&);
}

#endif
