#include "RenderWindow.h"
#include <algorithm>

RenderWindow::RenderWindow(GLint width, GLint height, const char *name, bool resizeable, bool windowed)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if(resizeable)
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	else
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwWindowHint(GLFW_SAMPLES, 4);

	if(windowed)
		window = glfwCreateWindow(width, height, name, nullptr, nullptr); //Windowed
	else
		window = glfwCreateWindow(width, height, name, glfwGetPrimaryMonitor(), nullptr); // Fullscreen

	glfwMakeContextCurrent(window);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
}



void RenderWindow::addViewPort(Camera* cam, GLfloat left, GLfloat top, GLfloat width, GLfloat height, GLfloat zOrder, GLfloat r, GLfloat g, GLfloat b)
{
	this->viewports.push_back(Viewport(cam, left, top, width, height, zOrder, r, g, b, this));
	std::sort(viewports.begin(), viewports.end());
}



void RenderWindow::setInputHandlers(GLFWkeyfun key, GLFWcursorposfun cursor, GLFWscrollfun scroll)
{
	glfwSetKeyCallback(window, key);
	glfwSetCursorPosCallback(window, cursor);
	glfwSetScrollCallback(window, scroll);
}



int RenderWindow::close()
{
	return glfwWindowShouldClose(window);
}



RenderWindow::~RenderWindow()
{
	glfwTerminate();
}



void RenderWindow::Update(SceneManager& scene, float seconds)
{
	scene.Update(seconds);
}


void RenderWindow::Render(SceneManager& scene)
{
	for(auto viewport : viewports)
	{
		viewport.Render(scene);
	}

	glfwSwapBuffers(window);
}


