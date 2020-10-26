#pragma once

#include <GL\glew.h>
#include "Maths.h"
#include <string>
#include <vector>

using namespace std;

class Shader
{
public:
	Shader();
	Shader(string path, vector<pair<int, string>> attributes);
	std::string readFile(char* path);
	GLuint program;
	void bind();
	void unBind();
	void setUniform(const char* name, mat4x4 value);
	void setUniform(const char* name, vector4 value);
	void setUniform(const char* name, vector3 value);
	void setUniform(const char* name, vector2 value);

	void bindAttribLocation(int n, char* name);

	void setUniform(const char* name, int value);
	void setUniform(const char* name, float value);
	void setUniform(std::string name, vector3* value);
	void setUniform(std::string name, mat4x4* value);
	~Shader();
};

