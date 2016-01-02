#include "Material.h"
#include "Camera.h"
#include "SceneManager.h"

Material::Material(const char *id, SceneManager *manager, const char *shaderId, shaderTypes shaderType, GLfloat* ambientI, GLfloat* diffuseI,
						GLfloat *specularI, GLfloat shininess, GLfloat opacity, int shadingModel, const char *diffuseId, const char *specularId, const char *normalId)
{
	this->id = id;
	this->shader = manager->getShader(shaderId);
	this->shaderType = shaderType;
	this->manager = manager;
	
	this->ambientIntensity = ambientI;
	this->diffuseIntensity = diffuseI;
	this->specularIntensity = specularI;
	this->shininess = shininess;
	this->opacity = opacity;
	this->shadingModel = shadingModel;
	
	if(diffuseId)
		textures.push_back(manager->getTexture(diffuseId));

	if(specularId)
		textures.push_back(manager->getTexture(specularId));

	if (normalId)
		textures.push_back(manager->getTexture(normalId));

	glGenTextures(1, &emissionMap);
}



Material::Material(const char* id, SceneManager* manager, const char* shaderId, shaderTypes shaderType, GLfloat* ambientI, GLfloat* diffuseI,
						GLfloat *specularI, GLfloat shininess, GLfloat opacity, int shadingModel, vector<Texture*> textures)
{
	this->id = id;
	this->shader = manager->getShader(shaderId);
	this->shaderType = shaderType;
	this->manager = manager;

	this->ambientIntensity = ambientI;
	this->diffuseIntensity = diffuseI;
	this->specularIntensity = specularI;
	this->shininess = shininess;
	this->opacity = opacity;
	this->shadingModel = shadingModel;

	this->textures = textures;

	glGenTextures(1, &emissionMap);
}



Material::~Material()
{
	glDeleteTextures(1, &emissionMap);
}



const char* Material::getId()
{
	return id;
}



Shader* Material::getShader()
{
	return shader;
}



vector<Texture*> Material::getDiffuseMaps()
{
	vector<Texture*> rtrn;
	for(auto t : textures)
	{
		if(t->type == "diffuse_texture")
		{
			rtrn.push_back(t);
		}
	}
	return rtrn;
}



vector<Texture*> Material::getSpecMaps()
{
	vector<Texture*> rtrn;
	for (auto t : textures)
	{
		if (t->type == "specular_texture")
		{
			rtrn.push_back(t);
		}
	}
	return rtrn;
}



GLuint Material::getEmissionMap()
{
	return emissionMap;
}



void Material::use(Camera *camera, bool shadowMap)
{
	if(!shadowMap)
	{
		shader->Use();

		if (shaderType == LIGHTING_TEXTURED)
		{
			GLint viewPosLoc = glGetUniformLocation(shader->Program, "viewPos");
			glUniform3f(viewPosLoc, camera->Position.x, camera->Position.y, camera->Position.z);

			glUniform1f(glGetUniformLocation(shader->Program, "material.shininess"), this->shininess);
			glUniform1f(glGetUniformLocation(shader->Program, "material.opacity"), this->opacity);

			GLuint diffuseNr = 1;
			GLuint specularNr = 1;
			GLuint normalNr = 1;
			for (GLuint i = 0; i < this->textures.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
												  // Retrieve texture number (the N in diffuse_textureN)
				stringstream ss;
				string number;
				string name = this->textures[i]->type;
				if (name == "diffuse_texture")
					ss << diffuseNr++; // Transfer GLuint to stream
				else if (name == "specular_texture")
				{
					ss << specularNr++; // Transfer GLuint to stream
				}
				else if (name == "normal_texture")
				{
					ss << normalNr++;
				}
				number = ss.str();

				glUniform1i(glGetUniformLocation(shader->Program, ("material." + name + number + "_active").c_str()), 1);
				glUniform1i(glGetUniformLocation(shader->Program, ("material." + name + number).c_str()), i);
				glBindTexture(GL_TEXTURE_2D, this->textures[i]->texture);
			}
			glActiveTexture(GL_TEXTURE0);

			glUniform1i(glGetUniformLocation(shader->Program, "material.shading_model"), this->shadingModel);

			if (ambientIntensity)
			{
				glUniform3f(glGetUniformLocation(shader->Program, "material.ambientI"), ambientIntensity[0], ambientIntensity[1], ambientIntensity[2]);
			}

			if (diffuseIntensity)
			{
				glUniform3f(glGetUniformLocation(shader->Program, "material.diffuseI"), diffuseIntensity[0], diffuseIntensity[1], diffuseIntensity[2]);
			}

			if (specularIntensity)
			{
				glUniform3f(glGetUniformLocation(shader->Program, "material.specularI"), specularIntensity[0], specularIntensity[1], specularIntensity[2]);
			}

			auto directionalLight = this->manager->getDirectionalLight();

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
				glUniformMatrix4fv(glGetUniformLocation(shader->Program, "dirLight.lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(*directionalLight->getLightSpaceMatrix()));

				glActiveTexture(GL_TEXTURE6);
				glUniform1i(glGetUniformLocation(shader->Program, "dirLight.shadowMap"), 6);
				glBindTexture(GL_TEXTURE_2D, directionalLight->getShadowMap());
				glActiveTexture(GL_TEXTURE0);
			}

			auto lights = this->manager->getActiveLights();

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
		}
		else if (shaderType == EMITTER)
		{
			if (diffuseIntensity)
			{
				glUniform3f(glGetUniformLocation(shader->Program, "diffuse"), diffuseIntensity[0], diffuseIntensity[1], diffuseIntensity[2]);
			}
			else
			{
				glUniform3f(glGetUniformLocation(shader->Program, "diffuse"), 1.0, 1.0, 1.0);
			}
		}
	}
	else
	{
		this->manager->getShadowShader()->Use();
	}
	
}

void Material::unUse(Camera* camera, bool shadowMap)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr  = 1;
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)
		stringstream ss;
		string number;
		string name = this->textures[i]->type;
		if (name == "diffuse_texture")
			ss << diffuseNr++; // Transfer GLuint to stream
		else if (name == "specular_texture")
		{
			ss << specularNr++; // Transfer GLuint to stream
		}
		else if (name == "normal_texture")
		{
			ss << normalNr++;
		}
		number = ss.str();

		glBindTexture(GL_TEXTURE_2D, NULL);
		glUniform1i(glGetUniformLocation(shader->Program, ("material." + name + number + "_active").c_str()), 0);
	}
	glActiveTexture(GL_TEXTURE0);
}