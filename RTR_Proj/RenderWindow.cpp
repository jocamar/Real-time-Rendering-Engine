#include "RenderWindow.h"

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



RenderWindow::~RenderWindow()
{
	glfwTerminate();
}



void RenderWindow::Render(SceneManager& scene)
{
	for(auto viewport : viewports)
	{
		viewport.Render(scene);
	}
}


