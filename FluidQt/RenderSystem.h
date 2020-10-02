#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Maths.h"
#include <QOpenGLFunctions_3_1>


class RenderSystem : protected QOpenGLFunctions_3_1 {

public:
	RenderSystem();
	void render(Texture*);
	void updateTexture(const char* data);
	void moveCamera(float, float);
	void zoom(float);
private:
	Shader shader;
	GLuint vao;
	GLuint vbo;
	vector2 camera;
	float scale;
};

