#include "Shader.h"

Shader::Shader() :
	compiled(false)
{}

Shader::Shader(const string& vertex, const string& fragment, bool raw = false) :
	compiled(false)
{
	if(!raw) load(vertex, fragment);
	else loadRaw(vertex, fragment);
}

Shader::~Shader()
{
	if(compiled) destroy();
}

/* Private methods */
void Shader::printLog(unsigned int shaderId)
{
	int len = 0;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &len);
	if(len > 0)
	{
		char* log = new char[len];
		int out = 0;
		glGetShaderInfoLog(shaderId, len, &out, log);
		string msg = string(log) == string() ? "Unknown" : string(log);
		delete[] log;
		Console::log("Error : \n%s\n", msg.c_str());
	}
	else
	{
		Console::log("Unknown error");
	}
}

void Shader::include(string& reference, string sourceFilename)
{
	std::vector<string> lines = Tokenizer::parseLines(reference);
	if(lines.size() > 0)
	{
		string temp, directory = Tokenizer::getDirectory(sourceFilename, true);
		for(size_t i = 0; i < lines.size(); i++)
		{
			if(lines[i].substr(0, strlen("#include")) == "#include")
			{
				vector<string> tokens = Tokenizer::tokenize(lines[i], ' ');

				/* Read sub file */
				if(tokens.size() > 1)
				{
					string newPath = directory + tokens[1];

					string content;
					if(!FileReader::read(newPath, content))
					{
						Console::log("Error reading included file");
						continue;
					}
					vector<string> sub_lines = Tokenizer::parseLines(content);
					if(sub_lines.size() == 0)
					{
						Console::log("Error reading referenced file / file is invalid");
						continue;
					}

					for(size_t t = 0; t < sub_lines.size(); t++)
					{
						temp += sub_lines[t] + "\n";
						Console::log("%s", sub_lines[t].c_str());
					}
					temp += "\n";
				}
				else
				{
					Console::log("Include directive without a path");
					continue;
				}

			}
			else
			{
				temp += lines[i];
				if(i != lines.size()-1) temp += "\n";
			}
		}

		reference = temp;
	}
	else
	{
		Console::log("Included / referenced file is invalid");
	}
}

void Shader::destroy()
{
	glDetachShader(shader, fsh);
	glDetachShader(shader, vsh);

	glDeleteProgram(shader);
	glDeleteShader(fsh);
	glDeleteShader(vsh);
	compiled = false;
}

/* Public methods */

/* loads a vertex and a fragment shader into the program */
bool Shader::load(string vshFilename, string fshFilename)
{
	this->vshFilename = vshFilename;
	this->fshFilename = fshFilename;
	string vshSource, fshSource;
	/* read from file */
	if(!FileReader::read(vshFilename, vshSource))
	{
		Console::log("File is invalid [%s]", vshFilename.c_str());
		return false;
	}
	if(!FileReader::read(fshFilename, fshSource))
	{
		Console::log("File is invalid [%s]", fshFilename.c_str());
		return false;
	}

	include(vshSource, vshFilename);
	include(fshSource, fshFilename);

	return loadRaw(vshSource, fshSource);
}

bool Shader::loadRaw(string vertex, string fragment)
{
	if(!glCreateShader)
	{
		Console::log("Shaders are not supported on your graphics card");
		return false;
	}

	if(compiled) destroy();
	vsh = glCreateShader(GL_VERTEX_SHADER);
	fsh = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vshs = vertex.c_str();
	const char* fshs = fragment.c_str();
	glShaderSource(vsh, 1, &vshs, 0);
	glShaderSource(fsh, 1, &fshs, 0);

	/* compile shaders */
 	auto compile = 0;
	glCompileShader(vsh);
	glGetShaderiv(vsh, GL_COMPILE_STATUS, &compile);
	if(!compile)
	{
		if(vshFilename != string())
			Console::log("Compilation error [%s]", vshFilename.c_str());
		printLog(vsh);
		glDeleteShader(vsh);
		return false;
	}

	glCompileShader(fsh);
	glGetShaderiv(fsh, GL_COMPILE_STATUS, &compile);
	if(!compile)
	{
		if(fshFilename != string())
			Console::log("Compilation error [%s]", fshFilename.c_str());
		printLog(fsh);
		glDeleteShader(vsh);
		glDeleteShader(fsh);
		return false;
	}

	/* create program and link */
	shader = glCreateProgram();
	glAttachShader(shader, fsh);
	glAttachShader(shader, vsh);
	glLinkProgram(shader);
	return (compiled = true);
}

void Shader::bind()
{
	if(compiled) glUseProgram(shader);
}

void Shader::unbind()
{
	glUseProgram(0);
}

bool Shader::isCompiled()
{
	return compiled;
}

/* uniform pass */
/* float */
void Shader::valuef(const char* var, float value)
{
	glUniform1f(glGetUniformLocation(shader, var), value);
}

void Shader::vec2f(const char* var, float v1, float v2)
{
	glUniform2f(glGetUniformLocation(shader, var), v1, v2);
}

void Shader::vec3f(const char* var, float v1, float v2, float v3)
{
	glUniform3f(glGetUniformLocation(shader, var), v1, v2, v3);
}

void Shader::vec4f(const char* var, float v1, float v2, float v3, float v4)
{
	glUniform4f(glGetUniformLocation(shader, var), v1, v2, v3, v4);
}

/* int */
void Shader::valuei(const char* var, int value)
{
	glUniform1i(glGetUniformLocation(shader, var), value);
}

void Shader::vec2i(const char* var, int v1, int v2)
{
	glUniform2i(glGetUniformLocation(shader, var), v1, v2);
}

void Shader::vec3i(const char* var, int v1, int v2, int v3)
{
	glUniform3i(glGetUniformLocation(shader, var), v1, v2, v3);
}

void Shader::vec4i(const char* var, int v1, int v2, int v3, int v4)
{
	glUniform4i(glGetUniformLocation(shader, var), v1, v2, v3, v4);
}

/* glm */
void Shader::vec2f(const char* var, vec2 vec)
{
	vec2f(var, vec.x, vec.y);
}

void Shader::vec3f(const char* var, vec3 vec)
{
	vec3f(var, vec.x, vec.y, vec.z);
}

void Shader::vec4f(const char* var, vec4 vec)
{
	vec4f(var, vec.x, vec.y, vec.z, vec.w);
}

void Shader::mat3x3(const char* var, mat3 matrix)
{
	glUniformMatrix3fv(glGetUniformLocation(shader, var), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::mat4x4(const char* var, mat4 matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(shader, var), 1, GL_FALSE, glm::value_ptr(matrix));
}

/* Attributes (Shader) */
int Shader::getAttribute(const char* attrib)
{
	return glGetAttribLocation(shader, attrib);
}
