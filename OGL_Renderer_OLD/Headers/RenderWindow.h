#pragma once
#include "Globals.h"
#include "Viewport.h"

/**
* Class representing a rendering window with possibly several viewports.
*/
class RenderWindow
{
protected:
	vector<Viewport> viewports;
	GLFWwindow* window;

public:

	/**
	* Render window constructor.
	*
	* @param width is the window with.
	* @param height is the window height.
	* @param name is the window name.
	* @param resizeable specifies whether the window is fixed or resizeable.
	* @param windowed specifies whether the window is fullscreen or windowed.
	*/
	RenderWindow(GLint width, GLint height, const char *name, bool resizeable, bool windowed);

	/**
	* Adds a viewport to the render window.
	*
	* @param cam is the camera used by this viewport to render the scene.
	* @param left is the left-most pixel of this viewport on the render window.
	* @param top is the top pixel of this viewport on the render window.
	* @param width is the width of the viewport on the render window.
	* @param height is the height of the viewport on the render window.
	* @param zOrder is the order in which this viewport is rendered relative to other viewports (higher is rendered above).
	* @param r is the red background color value of this viewport.
	* @param g is the green background color value of this viewport.
	* @param b is the blue background color value of this viewport.
	*/
	Viewport* addViewPort(Camera *cam, GLfloat left, GLfloat top, GLfloat width, GLfloat height, GLfloat zOrder, GLfloat r = 0.0f, GLfloat g = 0.0f, GLfloat b = 0.0f);

	/**
	* Sets the input handlers for this window.
	*
	* @param key is the function to be called when a key is pressed.
	* @param cursor is the function to be called when the cursor moves.
	* @param scroll is the function to be called when the mouse-wheel is scrolled.
	*/
	void setInputHandlers(GLFWkeyfun key, GLFWcursorposfun cursor, GLFWscrollfun scroll);

	/**
	* Closes this window.
	*/
	int close();

	/**
	* Render window destructor.
	*/
	~RenderWindow();

	/**
	* Updates the viewports and the scene.
	*
	* @param scene is the scene to update.
	* @param millis is the elapsed time in milliseconds since the last update.
	*/
	void update(SceneManager& scene, float millis);

	/**
	* Renders the specified scene to this window on each viewport.
	* @TODO find a way to reconcile this with the possibility of multiple cameras in the scenegraph for multiple viewports.
	*
	* @param scene is the scene to be rendered.
	*/
	void render(SceneManager& scene);
};