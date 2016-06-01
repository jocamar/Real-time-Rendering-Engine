#pragma once

#include "Camera.h"
#include "Scene Graph/SceneManager.h"

class RenderWindow;

/**
* Class representing a viewport in which a scene is rendered.
*/
class Viewport
{
protected:
	//Left most position of this viewport on the window.
	GLfloat left;

	//Top most position of this viewport on the window.
	GLfloat top;

	//Width of this viewport.
	GLfloat width;

	//Height of this viewport.
	GLfloat height;

	//Order in which this viewport is rendered compared to other viewports.
	GLfloat zOrder;

	//Render window to which this viewport is attached.
	RenderWindow *window;

	//Background color values of this viewport.
	GLfloat r;
	GLfloat g;
	GLfloat b;

	//VAO and VBO for the quad to which the final picture is rendered (used to apply post processing effects)
	GLuint quadVAO, quadVBO;

	//Framebuffer object to which the hdr values are rendered.
	GLuint HDRfbo;

	//Depth renderbuffer object to attach to the FBO
	GLuint DepthRbo;

	//Texture color buffers for the scene color and high intensity color
	GLuint textureColorbuffers[2];

	//Ping pong FBOs for the bloom effect.
	GLuint pingpongFbo[2];

	//Ping pong color buffers to attach to the FBOs
	GLuint pingpongColorbuffers[2];

	//Shader to render the final quad.
	Shader *screenShader;

	//Shader to blur the high intensity values.
	Shader *blurShader;

	//Specifies whether bloom is active.
	GLboolean bloom;

public:
	//Total milliseconds elapsed since start of execution.
	float totalMillis;
	
	//Exposure for this viewport.
	GLfloat exposure;

	//Camera attached to this viewport.
	Camera *camera;

	//Gamma value for this viewport.
	GLfloat gamma;

	/**
	* Viewport constructor.
	*
	* @param cam specifies the camera to attach to this viewport.
	* @param left specifies the left most pixel of this viewport in the window.
	* @param top specifies the top most pixel of this viewport in the window.
	* @param width specifies the width of this viewport.
	* @param height specifies the height of this viewport.
	* @param zOrder specifies the rendering order of this viewport.
	* @param r specifies the red color value of the background color of this viewport.
	* @param g specifies the green color value of the background color of this viewport.
	* @param b specifies the blue color value of the background color of this viewport.
	* @param window specifies the render window to which this viewport is attached.
	*/
	Viewport(Camera *cam, GLfloat left, GLfloat top, GLfloat width, GLfloat height, GLfloat zOrder, GLfloat r, GLfloat g, GLfloat b, RenderWindow *window);

	/**
	* Renders the specified scene from the point of view of this viewport.
	*
	* @param scene is the scene to render.
	*/
	void Render(SceneManager& scene);

	/**
	* Less-than operator compares this viewport to another according to their zOrder.
	*
	* @param v is the viewport to compare to.
	*/
	bool operator < (const Viewport& v) const;

	/**
	* Toggles the bloom effect on and off.
	*/
	void toggleBloom();

	/**
	* Auxiliar function to generate attachment tectures of differen types.
	* @TODO probably make private.
	*
	* @param depth specifies whether this texture is for a depth map.
	* @param stencil specifies whether this texture is for the stencil buffer.
	* @param interpMethod specifies the interpolation method to be used with this texture.
	*/
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil, GLint interpMethod = GL_REPEAT);
};