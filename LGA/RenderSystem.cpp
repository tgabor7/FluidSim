#include "RenderSystem.h"


RenderSystem::RenderSystem()
{
	scale = 1;
	shader = Shader("shaders/shader", { {0,"vertices"}});
	vao = Loader::createVao({0});

	GLfloat data[12] = {-1,1,1,1,-1,-1, // first triangle
						1,1,-1,-1,1,-1 // second triangle
						};

	vbo = Loader::storeDataInAttributeList(0, 2, data, 12);
}
void RenderSystem::render(Texture* t)
{
	shader.bind();
	shader.setUniform("camera", camera);
	shader.setUniform("scale", scale);
	//vertex array object
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);

	//texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t->getTextureBuffer());

	//draw
	glDrawArrays(GL_TRIANGLES, 0, 6);


	//clear
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);


	shader.unBind();
}
void RenderSystem::zoom(float s)
{
	scale += s;
}
void RenderSystem::moveCamera(float x, float y)
{
	camera.x -= x;
	camera.y += y;
}