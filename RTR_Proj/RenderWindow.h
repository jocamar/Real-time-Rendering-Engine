#pragma once
#include "Globals.h"
#include "Viewport.h"

class RenderWindow
{
protected:
	vector<Viewport> viewports;
	GLFWwindow* window;

public:
	RenderWindow(GLint width, GLint height, const char *name, bool resizeable, bool windowed);
	Viewport* addViewPort(Camera *cam, GLfloat left, GLfloat top, GLfloat width, GLfloat height, GLfloat zOrder, GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f);
	void setInputHandlers(GLFWkeyfun key, GLFWcursorposfun cursor, GLFWscrollfun scroll);
	int close();
	~RenderWindow();
	void update(SceneManager& scene, float millis);
	void render(SceneManager& scene);
};