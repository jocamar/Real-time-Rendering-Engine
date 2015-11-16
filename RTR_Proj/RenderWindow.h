#pragma once
#include "Viewport.h"
#include <GLFW/glfw3.h>

class RenderWindow
{
protected:
	vector<Viewport> viewports;
	GLFWwindow* window;

public:
	RenderWindow(GLint width, GLint height, const char *name, bool resizeable, bool windowed);
	~RenderWindow();
	void Render(SceneManager& scene);
};