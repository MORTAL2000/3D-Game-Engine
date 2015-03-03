#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "../IDE/UINode.h"
#include "../IDE/nodes/NumberNode.h"

class CodeGenerator
{
public:
	CodeGenerator();
	~CodeGenerator();

	bool generate(FILE*, UINode*, int);
	bool createCode(UINode*);
private:
	int m_cubes;
	int m_planes;
	int m_spheres;

	void writeln(FILE*, const std::string&);
	void writeln(FILE*);

	double readNumber(UINode*, int);
	double readNumber(UINode*, const std::string&);
	vec3 readVec3(UINode*, const std::string&);
};

#endif
