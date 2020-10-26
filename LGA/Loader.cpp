#include "Loader.h"
#include <iostream>


Loader::Loader()
{
}
GLuint Loader::createVao(vector<int> i) {
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	for (int k : i) {
		glEnableVertexAttribArray(k);
	}


	return vao;
}
GLuint Loader::createEmptyVBO(int floatCount)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, floatCount * 4, nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;
}
GLuint Loader::storeDataInAttributeList(int attributeNumber, int coordinateSize, const GLfloat  data[], int nFloats) {
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * nFloats, data, GL_STATIC_DRAW);
	glVertexAttribPointer(
		attributeNumber,
		coordinateSize,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	return vertexbuffer;
}
GLuint Loader::storeDataInDynamicAttributeList(int attributeNumber, int coordinateSize, const GLfloat data[], int nFloats) {
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * nFloats, data, GL_STATIC_DRAW);
	glVertexAttribPointer(
		attributeNumber,
		coordinateSize,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	return vertexbuffer;
}

GLuint Loader::storeDataInAttributeList(int attributeNumber, int coordinateSize, const GLuint  data[], int nFloats) {
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * nFloats, data, GL_STATIC_DRAW);
	glVertexAttribPointer(
		attributeNumber,
		coordinateSize,
		GL_INT,
		GL_FALSE,
		0,
		(void*)0
	);
	glVertexAttribIPointer(attributeNumber, coordinateSize, GL_INT, 0, 0);
	return vertexbuffer;
}
void Loader::addInstancedAttribute(int vao, int vbo, int attribute, int dataSize, int instancedDataLength, int offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindVertexArray(vao);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, false, instancedDataLength * 4, (void*)(offset * 4));
	glVertexAttribDivisor(attribute, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void Loader::updateVBO(int vbo, float data[], int length)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * length, data, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * length, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
GLuint Loader::bindIndicesBuffer(const GLint indices[], int nInts) {
	GLuint vboID;
	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, nInts * sizeof(GLint), indices, GL_STATIC_DRAW);
	return vboID;
}
void Loader::unBindVao() {
	glBindVertexArray(0);
}

Loader::~Loader()
{
}
