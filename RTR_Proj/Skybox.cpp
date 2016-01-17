#include "Skybox.h"
#include "SceneManager.h"
#include "Camera.h"

Skybox::Skybox(const char *id, SceneManager *manager) : Mesh(id)
{
	this->manager = manager;
	this->material = -1;

	enum { Vertices, /*Indexes,*/ numVBO };

	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}



Skybox::~Skybox()
{
}



void Skybox::display(glm::mat4 transf, int material, Camera *camera, bool shadowMap, Globals::LIGHT_TYPE shadowType) {

	int materialToUse;
	if (this->material >= 0)
		materialToUse = this->material;
	else
		materialToUse = material;

	auto s = this->manager->getMaterial(materialToUse);

	// Create camera transformation
	glm::mat4 mvm;
	if (!shadowMap)
	{
		glm::mat4 view;
		view = camera->GetViewMatrix();
		glm::mat4 projection;
		projection = camera->GetProjectionMatrix();
		mvm = projection * view * transf;
	}
	else
	{
		mvm = camera->ViewProjMatrix * transf;
	}
	// Pass the matrices to the shader
	if (!shadowMap)
	{
		glUniformMatrix4fv(s->getShader()->ModelViewLoc, 1, GL_FALSE, glm::value_ptr(mvm));
		glUniformMatrix4fv(s->getShader()->ModelLoc, 1, GL_FALSE, glm::value_ptr(transf));
	}
	else
	{
		if (shadowType == Globals::DIRECTIONAL) {
			glUniformMatrix4fv(manager->getShadowShader()->ModelViewLoc, 1, GL_FALSE, glm::value_ptr(mvm));
		}
		else if (shadowType == Globals::POINT)
		{
			for (GLuint i = 0; i < 6; ++i)
				glUniformMatrix4fv(glGetUniformLocation(manager->getOmniShadowShader()->Program, ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(camera->cubeViewProjectionMatrixes[i]));
			glUniform1f(glGetUniformLocation(manager->getOmniShadowShader()->Program, "far_plane"), 10.0f);
			glUniform3fv(glGetUniformLocation(manager->getOmniShadowShader()->Program, "lightPos"), 1, &(camera->getPosition()[0]));
			glUniformMatrix4fv(manager->getOmniShadowShader()->ModelLoc, 1, GL_FALSE, glm::value_ptr(transf));
		}
	}

	glBindVertexArray(skyboxVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}