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
	//glfwSwapInterval(1);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
}



Viewport* RenderWindow::addViewPort(Camera* cam, GLfloat left, GLfloat top, GLfloat width, GLfloat height, GLfloat zOrder, GLfloat r, GLfloat g, GLfloat b)
{
	this->viewports.push_back(Viewport(cam, left, top, width, height, zOrder, r, g, b, this));
	Viewport* rtrn = &(viewports[viewports.size() - 1]);
	std::sort(viewports.begin(), viewports.end());
	return rtrn;
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



void RenderWindow::update(SceneManager& scene, float millis)
{
	for (int i = 0; i < viewports.size(); i++)
	{
		Viewport* viewport = &(viewports[i]);
		if (viewport->totalMillis >= 6000 && viewport->totalMillis < 8000)
		{
			auto time = 8000 - viewport->totalMillis;
			auto dif = 5.0 - viewport->exposure;
			auto speed = dif / (float)time;
			viewport->exposure += (float)speed * millis;
		}
		else if (viewport->totalMillis >= 10000 && viewport->totalMillis < 13000)
		{
			auto time = 13000 - viewport->totalMillis;
			auto dif = 1.0 - viewport->exposure;
			auto speed = dif / (float)time;
			viewport->exposure += speed * millis;
		}
		else if (viewport->totalMillis >= 65000 && viewport->totalMillis < 70000)
		{
			auto time = 70000 - viewport->totalMillis;
			auto dif = 0.5 - viewport->exposure;
			auto speed = dif / (float)time;
			viewport->exposure += speed * millis;
		}
		else if (viewport->totalMillis >= 70000 && viewport->totalMillis < 73000)
		{
			auto time = 73000 - viewport->totalMillis;
			auto dif = 1.0 - viewport->exposure;
			auto speed = dif / (float)time;
			viewport->exposure += speed * millis;
		}

		viewport->totalMillis += millis;
	}

	scene.update(millis);
}



void RenderWindow::render(SceneManager& scene)
{
	for(auto viewport : viewports)
	{
		viewport.Render(scene);
	}

	glfwSwapBuffers(window);
}


