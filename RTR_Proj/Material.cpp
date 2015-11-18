#include "Material.h"
#include "Camera.h"
#include "SceneManager.h"

Material::Material(const char *id, const char *diffuse, const char *specular, Shader *shader, shaderTypes shaderType, SceneManager *manager)
{
	this->id = id;
	this->shader = shader;
	this->shaderType = shaderType;
	this->sceneManager = manager;
	
	glGenTextures(1, &diffuseMap);
	glGenTextures(1, &specularMap);
	glGenTextures(1, &emissionMap);

	int width, height;
	unsigned char* image;
	if(diffuse)
	{
		// Diffuse map
		image = SOIL_load_image(diffuse, &width, &height, 0, SOIL_LOAD_RGB);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	
	if(specular)
	{
		// Specular map
		image = SOIL_load_image(specular, &width, &height, 0, SOIL_LOAD_RGB);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
}



Material::~Material()
{
	glDeleteTextures(1, &diffuseMap);
	glDeleteTextures(1, &specularMap);
	glDeleteTextures(1, &emissionMap);
}


Shader* Material::getShader()
{
	return shader;
}



GLuint Material::getDiffuseMap()
{
	return diffuseMap;
}



GLuint Material::getSpecMap()
{
	return specularMap;
}



GLuint Material::getEmissionMap()
{
	return emissionMap;
}



void Material::use(Camera *camera)
{
	shader->Use();

	if(shaderType == LIGHTING_TEXTURED)
	{
		GLint viewPosLoc = glGetUniformLocation(shader->Program, "viewPos");
		glUniform3f(viewPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);

		glUniform1f(glGetUniformLocation(shader->Program, "material.shininess"), 32.0f); //parameterize this
		glUniform1i(glGetUniformLocation(shader->Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(shader->Program, "material.specular"), 1);

		auto directionalLight = this->sceneManager->getDirectionalLight();

		GLfloat* dif;
		GLfloat* amb;
		GLfloat* spec;
		GLfloat* dir;

		if (directionalLight) {
			dif = directionalLight->getDiffuse();
			amb = directionalLight->getAmbient();
			spec = directionalLight->getSpecular();
			dir = directionalLight->getDirection();

			glUniform3f(glGetUniformLocation(shader->Program, "dirLight.direction"), dir[0], dir[1], dir[2]);
			glUniform3f(glGetUniformLocation(shader->Program, "dirLight.ambient"), amb[0], amb[1], amb[2]);
			glUniform3f(glGetUniformLocation(shader->Program, "dirLight.diffuse"), dif[0], dif[1], dif[2]);
			glUniform3f(glGetUniformLocation(shader->Program, "dirLight.specular"), spec[0], spec[0], spec[0]);
		}

		auto lights = this->sceneManager->getActiveLights();

		glUniform1i(glGetUniformLocation(shader->Program, "activeLights"), lights.size());

		for (auto i = 0; i < MAX_LIGHTS && i < lights.size(); i++)
		{
			auto l = lights[i];
			auto pos = glm::vec3(l->getParent()->getTransfMatrix()[3]);
			dif = l->getDiffuse();
			amb = l->getAmbient();
			spec = l->getSpecular();
			auto constant = l->getConstant();
			auto linear = l->getLinear();
			auto quadratic = l->getQuadratic();

			glUniform3f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].position").c_str()), pos.x, pos.y, pos.z);
			glUniform3f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].ambient").c_str()), amb[0], amb[1], amb[2]);
			glUniform3f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].diffuse").c_str()), dif[0], dif[1], dif[2]);
			glUniform3f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].specular").c_str()), spec[0], spec[0], spec[0]);
			glUniform1f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].constant").c_str()), constant);
			glUniform1f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].linear").c_str()), linear);
			glUniform1f(glGetUniformLocation(shader->Program, ("pointLights[" + to_string(i) + "].quadratic").c_str()), quadratic);
		}

		// Bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		// Bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

	}
}