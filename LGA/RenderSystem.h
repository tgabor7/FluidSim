#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Loader.h"
#include "Maths.h"

class RenderSystem {

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

