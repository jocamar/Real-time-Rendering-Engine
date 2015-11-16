#include "Viewport.h"

Viewport::Viewport(Camera cam, GLfloat left, GLfloat top, GLfloat width, GLfloat height, std::shared_ptr<RenderWindow> window)
{
	this->camera = cam;
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->window = window;
}

void Viewport::Render(SceneManager& scene)
{
	scene.render(&camera);
}