#pragma once
#include <GL/glew.h>

class Texture
{
public:
	Texture() {}
	Texture(const char*, const int&, const int&);
	void update(const char* data);
	GLuint getTextureBuffer() const;
private:
	GLuint textureBuffer;
	int width;
	int height;
};

