#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "Shader.h"
#include <vector>

using namespace std;



Shader::Shader() {}
Shader::Shader(std::string path, vector<pair<int, string>> attributes)
{
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(path + ".vs", std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", (path + ".vs").c_str());
		getchar();

	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(path + ".fs", std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	program = glCreateProgram();
	glAttachShader(program, VertexShaderID);
	glAttachShader(program, FragmentShaderID);

	for (int i = 0; i < attributes.size(); i++) {
		glBindAttribLocation(program, attributes[i].first, attributes[i].second.c_str());
	}


	glLinkProgram(program);

	// Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &Result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glValidateProgram(program);
}
void Shader::bindAttribLocation(int n, char* name) {
	glBindAttribLocation(program, n, name);
}
void Shader::bind() {
	glUseProgram(program);
}
void Shader::unBind() {
	glUseProgram(0);
}
void Shader::setUniform(const char* name, vector4 value) {
	int location = glGetUniformLocation(program, name);
	if (location != -1)
		glUniform4f(location, value.x, value.y, value.z, value.w);
}
void Shader::setUniform(const char* name, vector3 value) {
	int location = glGetUniformLocation(program, name);
	if (location != -1)
		glUniform3f(location, value.x, value.y, value.z);
}
void Shader::setUniform(const char* name, vector2 value) {
	int location = glGetUniformLocation(program, name);
	if (location != -1)
		glUniform2f(location, value.x, value.y);
}
void Shader::setUniform(const char* name, int value) {
	int location = glGetUniformLocation(program, name);
	if (location != -1)
		glUniform1i(location, value);
}
void Shader::setUniform(const char* name, float value) {
	int location = glGetUniformLocation(program, name);
	if (location != -1)
		glUniform1f(location, value);
}
void Shader::setUniform(std::string name, vector3* values) {
	float* t = new float[4 * 3];
	int p = 0;
	for (int i = 0; i < 4; i++) {
		t[p] = values[i].x;
		p++;
		t[p] = values[i].y;
		p++;
		t[p] = values[i].z;
		p++;
	}
	glUniform3fv(glGetUniformLocation(program, name.c_str()), 4,
		t);

	delete[] t;

}
void Shader::setUniform(std::string name, mat4x4* values) {
	float* t = new float[16 * 4];
	int p = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 16; j++) {
			t[p] = values[i][j];
			p++;
		}
	}
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 4,
		false, t);
	delete[] t;
}
std::string Shader::readFile(char* path) {
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", path);
	}
	cout << VertexShaderCode;
	return VertexShaderCode;
}
void Shader::setUniform(const char* name, mat4x4 value)
{

	int location = glGetUniformLocation(program, name);
	if (location != -1)
		glUniformMatrix4fv(location, 1, false, value.getPointer());

}
Shader::~Shader()
{
}
