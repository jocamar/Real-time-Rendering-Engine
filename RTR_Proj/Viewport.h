#pragma once
#include <memory>

#include "Camera.h"
#include "SceneManager.h"

class RenderWindow;

class Viewport
{
protected:
	Camera camera;

	GLfloat left;
	GLfloat top;
	GLfloat width;
	GLfloat height;

	GLint actualLeft;
	GLint actualTop;
	GLint actualWidth;
	GLint actualHeight;

	GLfloat zOrder;

	std::shared_ptr<RenderWindow> window;
public:
	Viewport(Camera cam, GLfloat left, GLfloat top, GLfloat width, GLfloat height, std::shared_ptr<RenderWindow> window);
	void Render(SceneManager& scene);
};