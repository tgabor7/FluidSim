#pragma once
#include <QOpenGLFunctions_4_5_Core>

class Texture : protected QOpenGLFunctions_4_5_Core
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
	QOpenGLFunctions* context;
};

