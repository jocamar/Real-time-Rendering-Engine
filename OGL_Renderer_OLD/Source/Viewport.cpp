#include "Viewport.h"

Viewport::Viewport(Camera *cam, GLfloat left, GLfloat top, GLfloat width, GLfloat height, GLfloat zOrder, GLfloat r, GLfloat g, GLfloat b, RenderWindow *window)
{
	this->camera = cam;
	this->left = left;
	this->top = top;
	this->width = width;
	this->height = height;
	this->window = window;
	this->zOrder = zOrder;
	this->r = r;
	this->g = g;
	this->b = b;
	this->bloom = true;
	this->exposure = 1.0f;
	this->totalMillis = 0;
	this->gamma = 2.2f;

	GLfloat quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	//Generate VAO and VBO for the screen filling quad
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);

	//Generate FBO for the HDR values
	glGenFramebuffers(1, &HDRfbo);
	glBindFramebuffer(GL_FRAMEBUFFER, HDRfbo);
	// Create a color attachment texture
	textureColorbuffers[0] = generateAttachmentTexture(false, false, GL_CLAMP_TO_EDGE);
	textureColorbuffers[1] = generateAttachmentTexture(false, false, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffers[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, textureColorbuffers[1], 0);

	// Create a renderbuffer object
	glGenRenderbuffers(1, &DepthRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width, this->height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthRbo); 
				
	//Setup the two color attachments (for regular colors and high intensity colors
	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	//Check to see if FBO is correct
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Generate the two ping-pong buffers for the blur effect
	glGenFramebuffers(2, pingpongFbo);
	glGenTextures(2, pingpongColorbuffers);
	for (GLuint i = 0; i < 2; i++)
	{
		//Set up the attachments
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFbo[i]);
		pingpongColorbuffers[i] = generateAttachmentTexture(false, false, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);

		//Check if FBO is correct
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}

	//Set up clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Load shaders for the blur and the rendering of the screen filling quad
	screenShader = new Shader("Resources/Shaders/screen.vs", "Resources/Shaders/screen.frag");
	blurShader = new Shader("Resources/Shaders/blur.vs", "Resources/Shaders/blur.frag");
}



GLuint Viewport::generateAttachmentTexture(GLboolean depth, GLboolean stencil, GLint interpMethod)
{

	GLenum attachment_type, attachment_type2;

	//Determine the correct attachment type
	if (!depth && !stencil)
	{
		attachment_type = GL_RGB16F;
		attachment_type2 = GL_RGB;
	}
	else if (depth && !stencil)
	{
		attachment_type = GL_DEPTH_COMPONENT;
		attachment_type2 = attachment_type;
	}
	else if (!depth && stencil) {
		attachment_type = GL_STENCIL_INDEX;
		attachment_type2 = attachment_type;
	}

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth || !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, this->width, this->height, 0, attachment_type2, GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->width, this->height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(interpMethod != GL_REPEAT)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, interpMethod); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, interpMethod);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}



void Viewport::Render(SceneManager& scene)
{
	//Start by generating the shadow maps
	scene.generateShadowMaps(this->camera);

	//Set up the viewport
	glViewport(left, top, width, height);
	glScissor(left, top, width, height);
	glEnable(GL_SCISSOR_TEST);

	//Render the scene into the HDR buffer
	glBindFramebuffer(GL_FRAMEBUFFER, HDRfbo);

	// Clear all attached buffers        
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_CULL_FACE);
	scene.render(camera);
	glEnable(GL_CULL_FACE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Blur bright fragments w/ two-pass Gaussian Blur 
	GLboolean horizontal = true, first_iteration = true;
	GLuint amount = 10;
	blurShader->Use();
	for (GLuint i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFbo[horizontal]);
		glUniform1i(glGetUniformLocation(blurShader->Program, "horizontal"), horizontal);
		
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? textureColorbuffers[1] : pingpongColorbuffers[!horizontal]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	//Render the final image to the screen quad
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Clear all relevant buffers
	glCullFace(GL_BACK);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST); 

	screenShader->Use();
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, textureColorbuffers[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
	glUniform1i(glGetUniformLocation(screenShader->Program, "bloom"), bloom);
	glUniform1f(glGetUniformLocation(screenShader->Program, "exposure"), exposure);
	glUniform1f(glGetUniformLocation(screenShader->Program, "gamma"), gamma);
	glUniform1i(glGetUniformLocation(screenShader->Program, "scene"), 0);
	glUniform1i(glGetUniformLocation(screenShader->Program, "bloomBlur"), 1);
	
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}



bool Viewport::operator<(const Viewport& v) const
{
	return (this->zOrder < v.zOrder);
}

void Viewport::toggleBloom()
{
	bloom = !bloom;
}