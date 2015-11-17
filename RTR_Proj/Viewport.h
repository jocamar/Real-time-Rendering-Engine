#pragma once

#include "Camera.h"
#include "SceneManager.h"

class RenderWindow;

class Viewport
{
protected:
	Camera *camera;

	GLfloat left;
	GLfloat top;
	GLfloat width;
	GLfloat height;

	GLint actualLeft;
	GLint actualTop;
	GLint actualWidth;
	GLint actualHeight;

	GLfloat zOrder;

	RenderWindow *window;

	GLfloat r;
	GLfloat g;
	GLfloat b;
public:
	Viewport(Camera *cam, GLfloat left, GLfloat top, GLfloat width, GLfloat height, GLfloat zOrder, GLfloat r, GLfloat g, GLfloat b, RenderWindow *window);
	void Render(SceneManager& scene);
	bool operator < (const Viewport& v) const;
};