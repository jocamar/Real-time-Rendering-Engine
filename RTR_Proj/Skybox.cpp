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


	GLuint skyboxVAO, skyboxVBO;
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

	auto s = this->manager->getMaterial(material);

	s->use(camera, shadowMap, shadowType);

	// Create camera transformation
	glm::mat4 mvm;
	if (!shadowMap)
	{
		glm::mat4 view;
		view = camera->GetViewMatrix();
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera->Zoom), (float)1280 / (float)720, 0.1f, 1000.0f);
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
			glUniform3fv(glGetUniformLocation(manager->getOmniShadowShader()->Program, "lightPos"), 1, &(camera->Position[0]));
			glUniformMatrix4fv(manager->getOmniShadowShader()->ModelLoc, 1, GL_FALSE, glm::value_ptr(transf));
		}
	}

	vector<const GLchar*> faces;
	faces.push_back("skybox/right.jpg");
	faces.push_back("skybox/left.jpg");
	faces.push_back("skybox/top.jpg");
	faces.push_back("skybox/bottom.jpg");
	faces.push_back("skybox/back.jpg");
	faces.push_back("skybox/front.jpg");
	cubemapTexture = loadCubemap(faces);

	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);//, GL_UNSIGNED_BYTE, 0);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	s->unUse(camera, shadowMap, shadowType);
}


GLuint Skybox::loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}