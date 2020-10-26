#include <GL\glew.h>
#include <vector>
#pragma once

using namespace std;

class Loader
{
public:
	Loader();
	static GLuint createVao(vector<int> i);
	static GLuint createEmptyVBO(int floatCount);
	static GLuint storeDataInAttributeList(int attribNumber, int coordinateSize, const GLfloat data[], int nFloats);
	static GLuint storeDataInDynamicAttributeList(int attribNumber, int coordinateSize, const GLfloat data[], int nFloats);
	static GLuint storeDataInAttributeList(int attribNumber, int coordinateSize, const GLuint data[], int nFloats);
	static void addInstancedAttribute(int vao, int vbo, int attribute, int dataSize, int instancedDataLength, int offset);
	static void updateVBO(int vbo, float data[], int length);

	static GLuint bindIndicesBuffer(const GLint indices[], int nInts);
	static void unBindVao();

	~Loader();
};

