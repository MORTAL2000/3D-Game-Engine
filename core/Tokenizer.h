#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <sstream>
#include <string>
#include <vector>
#include <ctype.h>

namespace Tokenizer
{
	std::vector<std::string> tokenize(std::string, char);
	std::string getFileExtension(const std::string&);
	std::string getDirectory(const std::string&);
	std::string getDirectory(const std::string&, bool);
	std::string removePath(const std::string&);
	std::string removeWhitespace(const std::string&);
	std::vector<std::string> parseLines(const std::string&);
	std::string removeCharacter(const std::string&, char);
	std::vector<std::size_t> findAll(const std::string&, const std::string&);
	std::vector<std::size_t> findAllDigits(const std::string&);
}

#endif
