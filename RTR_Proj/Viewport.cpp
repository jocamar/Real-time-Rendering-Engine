#include "Viewport.h"

Viewport::Viewport(Camera *cam, GLfloat left, GLfloat top, GLfloat width, GLfloat height, GLfloat zOrder, GLfloat r, GLfloat g, GLfloat b, RenderWindow *window)
{
	this->camera = cam;
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->window = window;
	this->zOrder = zOrder;
	this->r = r;
	this->g = g;
	this->b = b;
}

void Viewport::Render(SceneManager& scene)
{
	glViewport(left, top, width, height);
	glScissor(left, top, width, height);
	glEnable(GL_SCISSOR_TEST);
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	scene.render(camera);
}

bool Viewport::operator<(const Viewport& v) const
{
	return (this->zOrder < v.zOrder);
}