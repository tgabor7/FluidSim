#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include "Shader.h"
#include <vector>

using namespace std;



Shader::Shader() {}
Shader::Shader(std::string path, vector<pair<int, string>> attributes)
{
	
	context = QOpenGLContext::currentContext()->functions();
	// Create the shaders
	GLuint VertexShaderID = context->glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = context->glCreateShader(GL_FRAGMENT_SHADER);

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
	printf("Compiling shader : %s\n", (path + ".vs").c_str());
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	context->glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	context->glCompileShader(VertexShaderID);

	// Check Vertex Shader
	context->glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	context->glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		context->glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", (path + ".fs").c_str());
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	context->glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	context->glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	context->glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	context->glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		context->glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	program = context->glCreateProgram();
	context->glAttachShader(program, VertexShaderID);
	context->glAttachShader(program, FragmentShaderID);

	for (int i = 0; i < attributes.size(); i++) {
		context->glBindAttribLocation(program, attributes[i].first, attributes[i].second.c_str());
	}


	context->glLinkProgram(program);

	// Check the program
	context->glGetProgramiv(program, GL_LINK_STATUS, &Result);
	context->glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		context->glGetProgramInfoLog(program, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	context->glValidateProgram(program);
}
void Shader::bindAttribLocation(int n, char* name) {
	context->glBindAttribLocation(program, n, name);
}
void Shader::bind() {
	context->glUseProgram(program);
}
void Shader::unBind() {
	context->glUseProgram(0);
}
void Shader::setUniform(const char* name, vector4 value) {
	int location = context->glGetUniformLocation(program, name);
	if (location != -1)
		context->glUniform4f(location, value.x, value.y, value.z, value.w);
}
void Shader::setUniform(const char* name, vector3 value) {
	int location = context->glGetUniformLocation(program, name);
	if (location != -1)
		context->glUniform3f(location, value.x, value.y, value.z);
}
void Shader::setUniform(const char* name, vector2 value) {
	int location = context->glGetUniformLocation(program, name);
	if (location != -1)
		context->glUniform2f(location, value.x, value.y);
}
void Shader::setUniform(const char* name, int value) {
	int location = context->glGetUniformLocation(program, name);
	if (location != -1)
		context->glUniform1i(location, value);
}
void Shader::setUniform(const char* name, float value) {
	int location = context->glGetUniformLocation(program, name);
	if (location != -1)
		context->glUniform1f(location, value);
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
	context->glUniform3fv(context->glGetUniformLocation(program, name.c_str()), 4,
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
	context->glUniformMatrix4fv(context->glGetUniformLocation(program, name.c_str()), 4,
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

	int location = context->glGetUniformLocation(program, name);
	if (location != -1)
		context->glUniformMatrix4fv(location, 1, false, value.getPointer());

}
Shader::~Shader()
{
}
