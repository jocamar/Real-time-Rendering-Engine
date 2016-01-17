#pragma once

#include "Camera.h"
#include "SceneManager.h"

class RenderWindow;

class Viewport
{
protected:
	GLfloat left;
	GLfloat top;
	GLfloat width;
	GLfloat height;

	GLfloat zOrder;

	RenderWindow *window;

	GLfloat r;
	GLfloat g;
	GLfloat b;

	GLuint quadVAO, quadVBO;
	GLuint HDRfbo;
	GLuint DepthRbo;
	GLuint textureColorbuffers[2];
	GLuint pingpongFbo[2];

	GLuint pingpongColorbuffers[2];

	Shader *screenShader;
	Shader *blurShader;

	GLboolean bloom;
public:
	GLfloat exposure;
	Camera *camera;
	Viewport(Camera *cam, GLfloat left, GLfloat top, GLfloat width, GLfloat height, GLfloat zOrder, GLfloat r, GLfloat g, GLfloat b, RenderWindow *window);
	void Render(SceneManager& scene);
	bool operator < (const Viewport& v) const;
	void toggleBloom();
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil, GLint interpMethod = GL_REPEAT);
};